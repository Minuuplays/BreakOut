#include "Headers.hpp"
//using namespace std;

void resetGame(vector<Brick*>& bricks, vector<PowerUp*>& powerups,
    Paddle* paddle, Ball* ball, int& score, int& lives, int& level,
    int SCRWIDTH, int SCRHEIGHT, LevelManager* levelManager);

void loadCurrentLevel(vector<Brick*>& bricks, LevelManager* levelManager,
    int brickWidth, int brickHeight, int padding, int xOffset, int yOffset);

int main(int argc, char* argv[]) {
    try {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
            throw runtime_error(string("SDL_Init Error: ") + SDL_GetError());
        }

        if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
            throw runtime_error(string("IMG_Init Error: ") + SDL_GetError());
        }

        std :: cout << "SDL initialized successfully!" << endl;

		const int SCRWIDTH = 1280;
		const int SCRHEIGHT = 720;
        RenderWindow window("BreakOut_v1.0", SCRWIDTH, SCRHEIGHT);

        // Game UI and Score
        GameUI* ui = new GameUI(window.getRenderer());
        ScoreManager* scoremanager = new ScoreManager("highscore.txt");

		// Level Manager
        LevelManager* levelManager = new LevelManager();
        cout << "Loaded " << levelManager->getTotalLevels() << " levels" << endl;


        // Loading Textures
        SDL_Texture* backGround = window.loadTexture("Assets/backGround.jpg");

        SDL_Texture* multiBalls = window.loadTexture("Assets/multiBalls.png");
        SDL_Texture* widePaddle = window.loadTexture("Assets/widePaddle.png");
        SDL_Texture* extraLife = window.loadTexture("Assets/extraLife.png");
        SDL_Texture* slowBall = window.loadTexture("Assets/slowBall.png");
        SDL_Texture* paddleLaser = window.loadTexture("Assets/laser.png");

        GameState gameState = GameState::MENU;
        int menuSelection = 0;
     
        // >> Dynamic Allocation << //
        Paddle* paddle = new Paddle((SCRWIDTH/2)-75,SCRHEIGHT-100,150,20,SCRWIDTH); 
		Ball* ball = new Ball(SCRWIDTH / 2, SCRHEIGHT - 110, 10.0f, SCRWIDTH, SCRHEIGHT, 350.0f);
        
		// >> STL Vector << //
        vector<Brick*> bricks;
        vector<PowerUp*> powerups;
        vector<Ball*> extraBalls;
        vector<Laser*> lasers;

        list<PowerUpEffect> activeEffects;

        const Uint32 WIDE_PADDLETIME = 10000; // for 10 seconds
        const Uint32 SLOW_BALLTIME = 7000;
        const Uint32 LASERTIME = 10000;

        Uint32 lastLaser = 0;
        const Uint32 LASER_COOLDOWN = 500; // 0.5 seconds between shots

        // Game Variables
        int score = 0;
        int lives = 3;
        int level = 1;

        resetGame(bricks, powerups, paddle, ball, score, lives, level, SCRWIDTH, SCRHEIGHT, levelManager);
        
        bool gameRunning = true;
        SDL_Event event;

        const int FPS = 60;
        const int frameDelay = 1000 / FPS;
        Uint32 frameStart;
        int frameTime;
		float deltaTime = 0.016f; // Approximate delta time for 60 FPS

		// Game loop
        while (gameRunning) {
            frameStart = SDL_GetTicks();

			// Event handling
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                }
                else if (event.type == SDL_KEYDOWN) {
                    switch (gameState) 
                    {
                    case GameState::MENU:
                        if (event.key.keysym.sym == SDLK_UP) { menuSelection = 0; }
                        else if (event.key.keysym.sym == SDLK_DOWN) { menuSelection = 1; }
                        else if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            if (menuSelection == 0)
                            {
                                gameState = GameState::PLAYING;
                            }
                            else 
                            {
                                gameRunning = false;
                            }
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            gameRunning = false;
                        }
                        break;
                    case GameState::PLAYING:
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            gameState = GameState::PAUSED;
                        }
                        else if (event.key.keysym.sym == SDLK_SPACE)
                        {
                            if (!ball->isLaunched())
                            {
                                ball->launch(60);
                            }
                        }
                        else if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_e)
                        {
                            if (paddle->canShoot())
                            {
                                Uint32 currentTime = SDL_GetTicks();

                                if (currentTime - lastLaser >= LASER_COOLDOWN)
                                {
                                    Laser* laser1 = new Laser(    // laft-laser
                                        paddle->getPosition().x + paddle->getWidth()/2 - 4,
                                        paddle->getPosition().y - 30);
                                    lasers.push_back(laser1);
                                   /* Laser* laser2 = new Laser(    // right-laser
                                        paddle->getPosition().x + paddle->getWidth() - 2.5f,
                                        paddle->getPosition().y - 20);
                                    lasers.push_back(laser2);*/
                                    lastLaser = currentTime;
                                }
                            }
                        }             
                        // Level navigation keys for testing ....
                        else if (event.key.keysym.sym == SDLK_n)  // Next level (for testing)
                        {
                            if (levelManager->nextLevel()) {
                                level = levelManager->getCurrentLevelNumber();
                                loadCurrentLevel(bricks, levelManager, 100, 30, 10, 90, 50);
                                ball->reset(paddle->getPosition().x + 75, SCRHEIGHT - 110);
                                cout << "Loaded next level: " << level << endl;
                            }
                        }
                        else if (event.key.keysym.sym == SDLK_p)  // Previous level (for testing)
                        {
                            if (levelManager->previousLevel()) {
                                level = levelManager->getCurrentLevelNumber();
                                loadCurrentLevel(bricks, levelManager, 100, 30, 10, 90, 50);
                                ball->reset(paddle->getPosition().x + 75, SCRHEIGHT - 110);
                                cout << "Loaded previous level: " << level << endl;
                            }
                        }
                        break;
                    case GameState::PAUSED:
                        if (event.key.keysym.sym == SDLK_SPACE)
                        {
                            gameState = GameState::PLAYING;
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            resetGame(bricks, powerups, paddle, ball, score, lives, level, SCRWIDTH, SCRHEIGHT, levelManager);
                            gameState = GameState::MENU;
                        }
                        break;
                    case GameState::GAMEOVER:
                    
                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            resetGame(bricks, powerups, paddle, ball, score, lives, level, SCRWIDTH, SCRHEIGHT, levelManager);
                            gameState = GameState::MENU;
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            gameRunning = false;
                        }
                        break;
                    case GameState::WIN:
                        if (event.key.keysym.sym == SDLK_RETURN)
                        {
                            // NEW: Try to load next level
                            if (levelManager->hasMoreLevels()) {
                                levelManager->nextLevel();
                                level = levelManager->getCurrentLevelNumber();
                                loadCurrentLevel(bricks, levelManager, 100, 30, 10, 90, 50);
                                ball->reset(paddle->getPosition().x + 75, SCRHEIGHT - 110);
                                // Reset power-ups and extra balls
                                deleteAndClear(powerups);
                                deleteAndClear(extraBalls);
                                activeEffects.clear();
                                paddle->resetWidth();
                                paddle->disableLaser();
                                gameState = GameState::PLAYING;
                                cout << "Advancing to level " << level << endl;
                            }
                            else {
                                // Game completed all levels!
                                resetGame(bricks, powerups, paddle, ball, score, lives, level, SCRWIDTH, SCRHEIGHT, levelManager);
                                gameState = GameState::MENU;
                            }
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            gameRunning = false;
                        }
                        break;
                    }
                }
            }

            // Update game objects only while playing
            if (gameState == GameState::PLAYING) {
                paddle->update(deltaTime);
                ball->update(deltaTime);

                if (!ball->isLaunched()) {
                    ball->reset(paddle->getPosition().x + 75, SCRHEIGHT - 110);
                }

                // ========================================
                // UPDATE POWERUPS
                // ========================================
                for (auto it = powerups.begin(); it != powerups.end(); ) {
                    (*it)->update(deltaTime);
                    if (!(*it)->isActive()) {
                        delete* it;
                        it = powerups.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // UPDATE EXTRA BALLS
                // ========================================
                for (auto it = extraBalls.begin(); it != extraBalls.end(); ) {
                    (*it)->update(deltaTime);
                    if ((*it)->getPosition().y > SCRHEIGHT) {
                        delete* it;
                        it = extraBalls.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // UPDATE LASERS (OUTSIDE BRICK LOOP!)
                // ========================================
                for (auto it = lasers.begin(); it != lasers.end(); ) {
                    (*it)->update(deltaTime);
                    if (!(*it)->isActive()) {
                        delete* it;
                        it = lasers.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // POWERUP-PADDLE COLLISION
                // ========================================
                for (auto it = powerups.begin(); it != powerups.end(); ) {
                    PowerUp* powerup = *it;
                    SDL_Rect powerupRect = powerup->getBounds();
                    SDL_Rect paddleRect = paddle->getBounds();

                    if (CollisionManager::checkAABB(powerupRect, paddleRect)) {
                        switch (powerup->getType()) {
                        case PowerUpType::ExtraLife:
                            lives++;
                            break;

                        case PowerUpType::MultiBall: {
                            for (int i = 0; i < 2; i++) {
                                Ball* newBall = new Ball(ball->getPosition().x,
                                    ball->getPosition().y, 10, SCRWIDTH, SCRHEIGHT);
                                newBall->launch(45 + i * 30);
                                extraBalls.push_back(newBall);
                            }
                            break;
                        }

                        case PowerUpType::SlowBall: {
                            activeEffects.remove_if([](const PowerUpEffect& e) {
                                return e.type == PowerUpType::SlowBall;
                                });
                            Vector2D vel = ball->getVelocity();
                            ball->setVelocity(vel * 0.7f);
                            for (Ball* extraBall : extraBalls) {
                                Vector2D v = extraBall->getVelocity();
                                extraBall->setVelocity(v * 0.7f);
                            }
                            activeEffects.push_back(PowerUpEffect(PowerUpType::SlowBall, SLOW_BALLTIME));
                            break;
                        }

                        case PowerUpType::WidePaddle: {
                            activeEffects.remove_if([](const PowerUpEffect& e) {
                                return e.type == PowerUpType::WidePaddle;
                                });
                            paddle->setWidth(200);
                            activeEffects.push_back(PowerUpEffect(PowerUpType::WidePaddle, WIDE_PADDLETIME));
                            break;
                        }

                        case PowerUpType::LaserPaddle: {
                            activeEffects.remove_if([](const PowerUpEffect& e) {
                                return e.type == PowerUpType::LaserPaddle;
                                });
                            paddle->enableLaser();
                            activeEffects.push_back(PowerUpEffect(PowerUpType::LaserPaddle, LASERTIME));
                            break;
                        }
                        }
                        delete powerup;
                        it = powerups.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // BALL-PADDLE COLLISION
                // ========================================
                CollisionManager::checkBallPaddle(ball, paddle);
                for (Ball* extraBall : extraBalls) {
                    CollisionManager::checkBallPaddle(extraBall, paddle);
                }

                // ========================================
                // BALL-BRICK COLLISION
                // ========================================
                for (Ball* extraBall : extraBalls) {
                    for (auto bit = bricks.begin(); bit != bricks.end(); ) {
                        Brick* brick = *bit;

                        // Skip if already destroyed
                        if (brick->isDestroyed()) {
                            ++bit;
                            continue;
                        }

                        if (CollisionManager::checkBallBrick(extraBall, brick)) {
                            if (brick->isDestroyed()) {
                                score += brick->getPoints()*2;

                                // Powerup drop code...
                                if (rand() % 100 < 27) {
                                    PowerUpType type = static_cast<PowerUpType>(rand() % 5);
                                    SDL_Texture* pwtex = nullptr;
                                    switch (type) {
                                    case PowerUpType::ExtraLife: pwtex = extraLife; break;
                                    case PowerUpType::MultiBall: pwtex = multiBalls; break;
                                    case PowerUpType::LaserPaddle: pwtex = paddleLaser; break;
                                    case PowerUpType::WidePaddle: pwtex = widePaddle; break;
                                    case PowerUpType::SlowBall: pwtex = slowBall; break;
                                    }
                                    powerups.push_back(new PowerUp(
                                        brick->getPosition().x + brick->getWidth() / 2 - 20,
                                        brick->getPosition().y, type, pwtex));
                                }

                                delete brick;
                                bit = bricks.erase(bit);
                            }
                            else {
                                ++bit;
                            }
                        }
                        else {
                            ++bit;
                        }
                    }
                }

                for (auto it = bricks.begin(); it != bricks.end(); ) {
                    Brick* brick = *it;

                    if (CollisionManager::checkBallBrick(ball, brick)) {
                        if (brick->isDestroyed()) {
                            score += brick->getPoints()*2;

                            // 32% chance to drop powerup
                            if (rand() % 100 < 32) {
                                PowerUpType type = static_cast<PowerUpType>(rand() % 5);
                                SDL_Texture* pwtex = nullptr;
                                switch (type) {
                                case PowerUpType::ExtraLife: pwtex = extraLife; break;
                                case PowerUpType::MultiBall: pwtex = multiBalls; break;
                                case PowerUpType::LaserPaddle: pwtex = paddleLaser; break;
                                case PowerUpType::WidePaddle: pwtex = widePaddle; break;
                                case PowerUpType::SlowBall: pwtex = slowBall; break;
                                }
                                PowerUp* powerup = new PowerUp(
                                    brick->getPosition().x + brick->getWidth() / 2 - 20,
                                    brick->getPosition().y, type, pwtex);
                                powerups.push_back(powerup);
                            }
                        }
                    }

                    brick->update(deltaTime);

                    if (brick->isDestroyed()) {
                        delete brick;
                        it = bricks.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // LASER-BRICK COLLISION (OUTSIDE BRICK LOOP!)
                // ========================================
                for (auto lit = lasers.begin(); lit != lasers.end(); ) {
                    Laser* laser = *lit;
                    bool hit = false;

                    for (auto bit = bricks.begin(); bit != bricks.end(); ) {
                        Brick* brick = *bit;

                        if (CollisionManager::checkAABB(laser->getBounds(), brick->getBounds())) {
                            brick->takeDamage(1);
                            laser->setActive(false);
                            hit = true;

                            if (brick->isDestroyed()) {
                                score += brick->getPoints()*2;

                                // 15% chance spawn powerup
                                if (rand() % 100 < 15) {
                                    PowerUpType type = static_cast<PowerUpType>(rand() % 5);
                                    SDL_Texture* pwtex = nullptr;
                                    switch (type) {
                                    case PowerUpType::ExtraLife: pwtex = extraLife; break;
                                    case PowerUpType::MultiBall: pwtex = multiBalls; break;
                                    case PowerUpType::LaserPaddle: pwtex = paddleLaser; break;
                                    case PowerUpType::WidePaddle: pwtex = widePaddle; break;
                                    case PowerUpType::SlowBall: pwtex = slowBall; break;
                                    }
                                    powerups.push_back(new PowerUp(
                                        brick->getPosition().x + brick->getWidth() / 2 - 20,
                                        brick->getPosition().y, type, pwtex));
                                }

                                delete brick;
                                bit = bricks.erase(bit);
                            }
                            else {
                                ++bit;
                            }
                            break;  // Laser hit something, stop checking
                        }
                        else {
                            ++bit;
                        }
                    }

                    if (hit || !laser->isActive()) {
                        delete laser;
                        lit = lasers.erase(lit);
                    }
                    else {
                        ++lit;
                    }
                }

                // ========================================
                // UPDATE ACTIVE POWERUP EFFECTS
                // ========================================
                for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
                    if (it->isExpired()) {
                        switch (it->type) {
                        case PowerUpType::WidePaddle:
                            paddle->resetWidth();
                            break;
                        case PowerUpType::SlowBall: {
                            Vector2D vel = ball->getVelocity();
                            ball->setVelocity(vel * (1.0f / 0.7f));
                            for (Ball* extraBall : extraBalls) {
                                Vector2D v = extraBall->getVelocity();
                                extraBall->setVelocity(v * (1.0f / 0.7f));
                            }
                            break;
                        }
                        case PowerUpType::LaserPaddle:
                            paddle->disableLaser();
                            break;
                        default:
                            break;
                        }
                        it = activeEffects.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                // ========================================
                // CHECK WIN CONDITION
                // ========================================
                if (bricks.empty()) {
                    cout << "You Win! Final Score: " << score << endl;
                    scoremanager->updateHighScore(score);
                    gameState = GameState::WIN;
                }

                // ========================================
                // CHECK LOSE CONDITION
                // ========================================
                if (ball->getPosition().y > SCRHEIGHT) {
                    lives--;
                    cout << "Ball Missed! Lives left: " << lives << endl;
                    if (lives <= 0) {
                        cout << "Game Over! Final Score: " << score << endl;
                        scoremanager->updateHighScore(score);
                        gameState = GameState::GAMEOVER;
                    }
                    else {
                        ball->reset(paddle->getPosition().x + 75, SCRHEIGHT - 110);
                    }
                }
            }
         
			// Rendering
            window.clear();
            window.render(backGround);
            
            if (gameState == GameState::PLAYING || gameState == GameState::PAUSED)
            {
                for (Brick* brick : bricks) {
                    brick->render(window.getRenderer());
                }
                for (PowerUp* powerup : powerups) {
                    powerup->render(window.getRenderer());
                }
               
                for (Laser* laser : lasers) {
                    laser->render(window.getRenderer());
                }
                paddle->render(window.getRenderer()); // Drawing Paddle
                ball->render(window.getRenderer());   // Drawing Ball
                for (Ball* extraBall : extraBalls) {
                    extraBall->render(window.getRenderer());
                }

                ui->renderHUD(score, lives, level, scoremanager->getHighScore());

                int yOffset = 90;
                for (const PowerUpEffect& effect : activeEffects) {
                    string effectName;
                    switch (effect.type) {
                    case PowerUpType::WidePaddle: effectName = "Wide Paddle"; break;
                    case PowerUpType::SlowBall: effectName = "Slow Ball"; break;
                    case PowerUpType::LaserPaddle: effectName = "Laser"; break;
                    default: effectName = "Effect"; break;
                    }

                    string timerText = effectName + ": " + to_string((int)effect.getRemainingTime()) + "s";
                    SDL_Color yellow = { 255, 255, 0, 255 };
                    ui->renderText(timerText, 20, yOffset, yellow);
                    yOffset += 30;
                }

                if (gameState == GameState::PAUSED) {
                    ui->renderPauseScreen();
                }

                if (gameState == GameState::PAUSED)
                {
                    ui->renderPauseScreen();
                }
            }
            else if (gameState == GameState::MENU)
            {
                ui->renderMenu(menuSelection);
            }
            else if (gameState == GameState::GAMEOVER)
            {
                bool isNew = scoremanager->isNewHighScore(score);
                ui->renderGameOverScreen(score, isNew);
            }
            else if (gameState == GameState::WIN)
            {
                // Check if there are more levels
                if (levelManager->hasMoreLevels()) {
                    ui->renderWinScreen(score);  // Normal win screen - go to next level
                }
                else {
                    ui->renderGameWinScreen(score);  // Final win screen - completed all levels!
                }
            }
            
            window.display();

			// Frame rate control
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime) {
                SDL_Delay(frameDelay - frameTime);
            }
        }
		// Cleanup
        /*for_each(powerups.begin(), powerups.end(), [](PowerUp* powerup) { delete powerup; });
        powerups.clear();
        for_each(bricks.begin(), bricks.end(), [](Brick* brick) { delete brick; });
		bricks.clear();*/
       
        deleteAndClear(bricks);
        deleteAndClear(powerups);
        deleteAndClear(extraBalls);
        deleteAndClear(lasers);

        safeDelete(ball);
        safeDelete(paddle);
        safeDelete(ui);
        safeDelete(scoremanager);
		safeDelete(levelManager);

		window.cleanupTexture(multiBalls);
        window.cleanupTexture(widePaddle);
		window.cleanupTexture(extraLife);
		window.cleanupTexture(slowBall);
        window.cleanupTexture(paddleLaser);
        window.cleanupTexture(backGround);

        IMG_Quit();
        SDL_Quit();

		std :: cout << "Game exited successfully!" << endl;
    }
	// >> Exception Handling << //
    catch (const std::exception& e) {
        cerr << "FATAL ERROR: " << e.what() << endl;
		IMG_Quit();
        SDL_Quit();
        return 1;
    }
    return 0;
}

void resetGame(vector<Brick*>& bricks, vector<PowerUp*>& powerups,
    Paddle* paddle, Ball* ball, int& score, int& lives, int& level,
    int SCRWIDTH, int SCRHEIGHT, LevelManager* levelManager)
{
    /*for (Brick* brick : bricks) delete brick;
    for_each(powerups.begin(), powerups.end(), [](PowerUp* powerup) { delete powerup; });
    bricks.clear();
    powerups.clear*/

    deleteAndClear(bricks);
    deleteAndClear(powerups);

    score = 0;
    lives = 3;
    level = 1;

	levelManager->setLevel(0);

    paddle->setPosition(Vector2D(SCRWIDTH / 2 - 75, SCRHEIGHT - 100));
    paddle->setWidth(150);
    paddle->setVelocity(Vector2D(0, 0));
    paddle->setActive(true);

    ball->reset(SCRWIDTH / static_cast<float>(2), SCRHEIGHT - 110);
    ball->setActive(true);
    ball->setLaunched(false);
    ball->setVelocity(Vector2D(0, 0));

    loadCurrentLevel(bricks, levelManager, 100, 30, 10, 90, 50);
    /*
    const int BR_ROWS = 8;
    const int BR_COLS = 10;
    const int BR_WIDTH = 100;
    const int BR_HEIGHT = 30;
    const int BR_PADDING = 10;
    const int BR_X_OFFSET = 90;
    const int BR_Y_OFFSET = 50;

    for(int row = 0;row<BR_ROWS; row++) {
        for (int cols = 0;cols < BR_COLS; cols++) {
             float x = BR_X_OFFSET + cols * (BR_WIDTH + BR_PADDING);
             float y = BR_Y_OFFSET + row * (BR_HEIGHT + BR_PADDING);

             int hp = ceil((BR_ROWS - row) / 3.0f); // Higher rows have more HP

             Brick* brick = new Brick(x, y, BR_WIDTH, BR_HEIGHT, hp);
             bricks.push_back(brick);
        }
    }
    */
 }

void loadCurrentLevel(vector<Brick*>& bricks, LevelManager* levelManager,
    int brickWidth, int brickHeight, int padding, int xOffset, int yOffset)
{
    levelManager->createBricksFromLevel(bricks, brickWidth, brickHeight,
        padding, xOffset, yOffset);
}