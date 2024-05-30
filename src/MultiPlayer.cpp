//
// Created by Paul Baudinot on 29/05/2024.
//

#include "../include/MultiPlayer.h"

using json = nlohmann::json;

void receive_messages(int sock, bool* running, std::vector<Monster*>* listMonster, Textures T, Player* player) {
    char buffer[1024];
    while (*running) {
        int valread = read(sock, buffer, 1024);
        if (valread > 0) {
            buffer[valread] = '\0';

            std::cout << "Received from server: " << buffer << std::endl;
            std::string jsonStr(buffer, valread);

            try {
                // Deserialize the string into a JSON object
                json receivedData = json::parse(jsonStr);

                if (receivedData.contains("data") && receivedData["data"].is_string()) {
                    std::string dataType = receivedData["data"];

                    if (dataType == "AddPlayer") {
                        if (receivedData.contains("position") && receivedData["position"].contains("x") &&
                            receivedData["position"]["x"].is_number() &&
                            receivedData["position"].contains("y") && receivedData["position"]["y"].is_number() &&
                            receivedData.contains("id") && receivedData["id"].is_number()) {

                            Monster *monster = new Monster{receivedData["position"]["x"], receivedData["position"]["y"],
                                                           T.monsterTexture}; // Dynamic allocation
                            monster->id = receivedData["id"];
                            listMonster->push_back(monster);
                        } else {
                            std::cerr << "Received 'AddPlayer' JSON does not contain valid 'position' or 'id' data"
                                      << std::endl;
                        }
                    } else if (dataType == "MovePlayer") {
                        if (receivedData.contains("position") && receivedData["position"].contains("x") &&
                            receivedData["position"]["x"].is_number() &&
                            receivedData["position"].contains("y") && receivedData["position"]["y"].is_number() &&
                            receivedData.contains("id") && receivedData["id"].is_number()) {

                            int id = receivedData["id"];
                            for (auto &monster: *listMonster) {
                                if (monster->id == id) {
                                    // Update the monster's position
                                    monster->posX = receivedData["position"]["x"];
                                    monster->posY = receivedData["position"]["y"];
                                    break;
                                }
                            }
                        }
                    } else if (dataType == "RemovePlayer") {
                        if (receivedData.contains("id") && receivedData["id"].is_number()) {
                            int id = receivedData["id"];
                            auto it = std::remove_if(listMonster->begin(), listMonster->end(), [id](Monster *monster) {
                                if (monster->id == id) {
                                    delete monster;  // Free the allocated memory
                                    return true;
                                }
                                return false;
                            });
                            if (it != listMonster->end()) {
                                listMonster->erase(it, listMonster->end());
                                std::cout << "Removed player with id " << id << std::endl;
                            }
                        }
                    } else if (dataType == "ShotPlayer") {
                        if (receivedData.contains("pv") && receivedData["pv"].is_number()) {
                            player->pv = receivedData["pv"];

                        }
                    }else {
                        std::cerr << "Received 'MovePlayer' JSON does not contain valid 'position' or 'id' data"
                                  << std::endl;
                    }
                }
                else {
                    std::cerr << "Received unknown data type in JSON" << std::endl;
                }
            } catch (json::parse_error& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
            }
        } else if (valread == 0) {
            std::cout << "Server closed the connection." << std::endl;
            *running = false;
            break;
        } else {
            perror("read");
            *running = false;
            break;
        }
    }
}

int connexionServer() {
    int sock = 0;
    struct sockaddr_in serv_addr;


    // Créer le socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir les adresses IPv4 et IPv6 de texte en binaire
    if (inet_pton(AF_INET, "51.91.97.234", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(sock); // Fermer le socket avant de retourner
        return -1;
    }

    // Connecter au serveur
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        close(sock); // Fermer le socket avant de retourner
        return -1;
    } else{
        std::cout << "Connection success" << std::endl;
    }

    return sock;
}

int multi(SDL_Renderer* renderer, TTF_Font* font, Textures T) {
    score = 0;

    int sock = connexionServer();
    if (sock < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        //return 1;
    }

    bool threadRunning = true;
    std::vector<Monster*> listMonster;



    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Player player{200, 150, &listMonster};

    std::thread receiver(receive_messages, sock, &threadRunning, &listMonster, T, &player);

    player.initMulti(sock);

    bool click = false;
    bool annimationBoucle = false;
    auto now = std::chrono::steady_clock::now();

    player.wallTexture = T.wallTexture;

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_q:
                        case SDLK_LEFT:
                            if (!isCollision(player.posX + sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY - cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += sin(player.angle * M_PI / 180) * vitesse;
                                player.posY -= cos(player.angle * M_PI / 180) * vitesse;
                                player.move(sock);
                            }
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            if (!isCollision(player.posX - sin(player.angle * M_PI / 180) * vitesse,
                                             player.posY + cos(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= sin(player.angle * M_PI / 180) * vitesse;
                                player.posY += cos(player.angle * M_PI / 180) * vitesse;
                                player.move(sock);
                            }
                            break;
                        case SDLK_z:
                        case SDLK_UP:
                            if (!isCollision(player.posX + cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY + sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX += cos(player.angle * M_PI / 180) * vitesse;
                                player.posY += sin(player.angle * M_PI / 180) * vitesse;
                                player.move(sock);
                            }
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            if (!isCollision(player.posX - cos(player.angle * M_PI / 180) * vitesse,
                                             player.posY - sin(player.angle * M_PI / 180) * vitesse)) {
                                player.posX -= cos(player.angle * M_PI / 180) * vitesse;
                                player.posY -= sin(player.angle * M_PI / 180) * vitesse;
                                player.move(sock);
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    player.angle += event.motion.xrel * sensitivity;
                    player.angle = fmod(player.angle, 360.0);
                    if (player.angle < 0) player.angle += 360.0;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    click = true;
            }
        }
        SDL_RenderClear(renderer);

        DisplayBackground(renderer, T.skyTexture, player.angle);
        player.view(renderer);
        player.lineCenter(renderer);
        DisplayPerso(player, renderer);

        DisplayScore(renderer, font);
        DisplayPv(renderer, font, player);

        if (click) {
            player.shotMulti(sock);
            now = std::chrono::steady_clock::now();
            annimationBoucle = true;
            click = false;
        }

        if (annimationBoucle) {
            AnnimShot(renderer, T, now, &annimationBoucle);
        } else {
            displayHUD(renderer, T.HUDTexture);
        }

        cursor(renderer);
        SDL_RenderPresent(renderer);

        if (player.pv <= 0) {
            threadRunning = false;
            close(sock);
            receiver.join();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DisplayText(renderer, font, "YOU ARE DEAD", {227, 20, 20});
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            break;
        }
    }

    if(threadRunning){
        threadRunning = false;
        close(sock);
        receiver.join();
    }
    return 0;
}