#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;
using namespace sf;

//=================general Variables======================
const float gravity = 550, jump_velocity = -700.0f;
Vector2f velocity(450.0f, 0);
int score = 0, game_starter = 0, highscore, x_daino1 = 0, x_daino2 = 1, current_frame = 0, spawn_counter = 0, random_selector, spawn_rand = 1, ground_level = 780, bird_index = 0;
float gravity_factor = 1, dino_timer = 0, timer = 0, delay = 0.1f;
bool is_bending = false, is_dead = false, isPauseMenuShown = false;

//menue variables and functions
bool isPaused = false, isSettingsOpen = false, boxVys = false, boxAlis = false,is_muted=false;
int game_state = 0; //0: at the start menu / 1: the game is playing
int selectedOption = 0; // for the menu
int alising = 8;


// a function for the options of the menu by the keyboard
void menufun(int& option, Event event) {

	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) {
		option = (option + 2) % 3;
	}
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down) {
		option = (option + 1) % 3;
	}
}

// hover effect function
void checkOption(int selectedOption, Text arr[], int x, int y, int z) {

	if (selectedOption == 0) {
		arr[x].setFillColor(Color::Black);
		arr[y].setFillColor(Color(128, 128, 128));
		arr[z].setFillColor(Color(128, 128, 128));

	}

	if (selectedOption == 1) {
		arr[x].setFillColor(Color(128, 128, 128));
		arr[y].setFillColor(Color::Black);
		arr[z].setFillColor(Color(128, 128, 128));
	}

	if (selectedOption == 2) {
		arr[x].setFillColor(Color(128, 128, 128));
		arr[y].setFillColor(Color(128, 128, 128));
		arr[z].setFillColor(Color::Black);
	}

}

// a function to check if the mouse is on the box or not
bool isMouseOver(RenderWindow& window, RectangleShape box[], int x) {

	float mouseX = Mouse::getPosition(window).x;
	float mouseY = Mouse::getPosition(window).y;

	float bxPosX = box[x].getPosition().x;
	float bxPosY = box[x].getPosition().y;

	float bxPosXwidth = box[x].getPosition().x + box[x].getLocalBounds().width;
	float bxPosYheight = box[x].getPosition().y + box[x].getLocalBounds().height;

	if (mouseX < bxPosXwidth && mouseX > bxPosX && mouseY < bxPosYheight && mouseY > bxPosY)
		return true;

	return false;
}

// a function to change selected option by the mouse
bool check = false;
void menuMouse(RectangleShape box[], int& selectedOption, RenderWindow& window, bool& check, int i) {

	for (i; i < 6; i++) {
		if (isMouseOver(window, box, i)) {
			selectedOption = i % 3;
			check = true;
			break;
		}
		else
			check = false;
	}


}

//settinds function 
void settingsFun(RenderWindow& window, RectangleShape settingsBoxes[], Event event,Sound& sound) {

	// hover effect at settings for the boxes and turn on the options at it
	if (isMouseOver(window, settingsBoxes, 2) && event.type == Event::MouseButtonPressed && !boxVys) {
		settingsBoxes[2].setFillColor(Color::Black);
		sound.play();
		boxVys = true;
	}
	else if (isMouseOver(window, settingsBoxes, 2) && event.type == Event::MouseButtonPressed && boxVys) {
		settingsBoxes[2].setFillColor(Color::White);
		sound.play();
		boxVys = false;

	}
	if (isMouseOver(window, settingsBoxes, 3) && event.type == Event::MouseButtonPressed && !boxAlis) {
		settingsBoxes[3].setFillColor(Color::Black);
		sound.play();
		boxAlis = true;
	}
	else if (isMouseOver(window, settingsBoxes, 3) && event.type == Event::MouseButtonPressed && boxAlis) {
		settingsBoxes[3].setFillColor(Color::White);
		sound.play();
		boxAlis = false;

	}

	if (isMouseOver(window, settingsBoxes, 6) && event.type == Event::MouseButtonPressed && !is_muted) {
		settingsBoxes[6].setFillColor(Color::Black);
		sound.play();
		is_muted = true;
	}
	else if (isMouseOver(window, settingsBoxes, 6) && event.type == Event::MouseButtonPressed && is_muted) {
		settingsBoxes[6].setFillColor(Color::White);
		sound.play();
		is_muted = false;
	}

	// back button
	if (isMouseOver(window, settingsBoxes, 4) && event.type == Event::MouseButtonPressed) {
		sound.play();
		isSettingsOpen = false;
	}

}

void SettingsCheck(bool antialiasing, bool Vsync, RectangleShape settings[], Window& window)
{
	if (antialiasing)
	{
		settings[3].setFillColor(Color::Black);
		antialiasing = 8;
	}
	else
		antialiasing = 0;

	if (Vsync)
		settings[2].setFillColor(Color::Black);

}

struct spawned
{
	Texture obs_texture[5];
	vector <Sprite> obs_sprite;
};

//RectangleShape Dinosaur(Vector2f(20.0f, 40.0f));


int main()
{
	//boxes for the menu
	RectangleShape menuboxes[8];

	for (int i = 0; i < 8; i++)
		menuboxes[i].setFillColor(Color::Transparent);

	//setting proper position for each menu box
	menuboxes[0].setSize(Vector2f{ 400,115 }); //start
	menuboxes[0].setPosition(295, 245);
	menuboxes[1].setSize(Vector2f{ 580,115 });//settings
	menuboxes[1].setPosition(230, 270 + 150);
	menuboxes[2].setSize(Vector2f{ 300,115 });//quit
	menuboxes[2].setPosition(350, 290 + 300);
	menuboxes[3].setSize(Vector2f{ 470,115 });//resume
	menuboxes[3].setPosition(680, 360);
	menuboxes[4].setSize(Vector2f{ 590,115 });//settings
	menuboxes[4].setPosition(620, 360 + 160);
	menuboxes[5].setSize(Vector2f{ 300,115 });//quit
	menuboxes[5].setPosition(750, 360 + 300);
	menuboxes[6].setSize(Vector2f{ 115,115 }); // box for game over
	menuboxes[6].setPosition(870, 600);
	menuboxes[7].setSize(Vector2f{ 300,115 }); // box for quit at game over
	menuboxes[7].setPosition(770, 750);
	//the fonts
	Font font;
	if (!font.loadFromFile("font-bold.ttf"))
		// Handle font loading error
		return EXIT_FAILURE;

	Text menu[7];
	for (int i = 0; i < 7; i++) {
		menu[i].setFont(font);
		menu[i].setFillColor(Color(128, 128, 128));
		menu[i].setCharacterSize(130);
	}

	//start text   
	menu[0].setPosition(285, 225);
	menu[0].setString(" Start");
	//settings text
	menu[1].setPosition(215, 240 + 150);
	menu[1].setString("Settings");
	// quit text
	menu[2].setPosition(350, 265 + 300);
	menu[2].setString(" Quit");
	//resume text
	menu[3].setPosition(680, 346);
	menu[3].setString("Resume");
	//settings text
	menu[4].setPosition(620, 346 + 150);
	menu[4].setString("Settings");
	// quit text
	menu[5].setPosition(730, 346 + 300);
	menu[5].setString(" Quit");
	// quit text for game over
	menu[6].setPosition(785, 730);
	menu[6].setString("Quit");

	// game title
	Text titletext("Google Dinosaur", font, 120);
	titletext.setFillColor(Color::Black);
	titletext.setPosition(400, 40);

	//instructions
	Text insttext("- Press The Space Key or Up Arrow To Jump", font, 25);
	Text Instructions_title("Instructions", font, 50);
	Instructions_title.setFillColor(Color::Black);
	Instructions_title.setPosition(1400, 285);
	insttext.setFillColor(Color::Black);
	insttext.setPosition(1300, 550);
	Text instructionstext("- Press The Down Arrow Key to Duck", font, 25);
	instructionstext.setFillColor(Color::Black);
	instructionstext.setPosition(1300, 700);
	Text paneltext("\n\n\nGoogle Dinosaur is a simple game where you \n need to avoid obstacles as much as you \n can to keep surviving", font, 25);
	paneltext.setFillColor(Color::Black);
	paneltext.setPosition(1300, 300);

	//Scores
	Text scores[2];
	scores->setFont(font);
	scores->setFillColor(Color(128,128,128));
	scores[0].setPosition(Vector2f(1600, 30));
	scores[1].setPosition(Vector2f(1400, 30));
	scores->setCharacterSize(50);

	//========================settings ==============================
	Text settingsText[5];
	for (int i = 1; i < 4; i++) {
		settingsText[i].setFont(font);
		settingsText[i].setFillColor(Color::Black);
		settingsText[i].setCharacterSize(60);
	}
	settingsText[0].setFont(font);
	settingsText[0].setFillColor(Color::Black);
	settingsText[0].setCharacterSize(80);
	settingsText[0].setPosition(350, 250);
	settingsText[0].setString("Settings");

	settingsText[1].setPosition(250, 400);
	settingsText[1].setString("Vysnc");

	settingsText[2].setPosition(250, 500);
	settingsText[2].setString("Anti alising");

	settingsText[3].setPosition(250, 600);
	settingsText[3].setString("Mute music");

	settingsText[4].setFont(font);
	settingsText[4].setFillColor(Color::Black);
	settingsText[4].setCharacterSize(80);
	settingsText[4].setPosition(400, 750);
	settingsText[4].setString("Back");

	// boxes for the settings
	RectangleShape settingsBoxes[7];
	for (int i = 0; i < 2; i++)
		settingsBoxes[i].setFillColor(Color::Black);

	for (int i = 2; i < 4; i++)
		settingsBoxes[i].setFillColor(Color::White);

	settingsBoxes[4].setFillColor(Color::Transparent);
	settingsBoxes[4].setSize(Vector2f{ 200,70 });
	settingsBoxes[4].setPosition(400, 770);

	settingsBoxes[0].setSize(Vector2f{ 70,70 }); 
	settingsBoxes[0].setPosition(700, 400);

	settingsBoxes[1].setSize(Vector2f{ 70,70 });
	settingsBoxes[1].setPosition(700, 500);

	settingsBoxes[2].setSize(Vector2f{ 50,50 });//vysnc box
	settingsBoxes[2].setPosition(710, 410);

	settingsBoxes[3].setSize(Vector2f{ 50,50 });//alising box
	settingsBoxes[3].setPosition(710, 510);

	//mute music boxes
	settingsBoxes[5].setFillColor(Color::Black);
	settingsBoxes[5].setSize(Vector2f{ 70,70 });
	settingsBoxes[5].setPosition(700, 600);
	settingsBoxes[6].setFillColor(Color::White);
	settingsBoxes[6].setSize(Vector2f{ 50,50 });
	settingsBoxes[6].setPosition(710, 610);


	//====================textures for menu =======================
	Texture texture[9];
	texture[0].loadFromFile("Background.png");
	texture[1].loadFromFile("dino.png");
	texture[2].loadFromFile("cactus1.png");
	texture[3].loadFromFile("birds.png");
	texture[4].loadFromFile("panel.png");
	texture[5].loadFromFile("instruction_4.png");
	texture[6].loadFromFile("replay.png");
	texture[7].loadFromFile("game-over.png");
	texture[8].loadFromFile("hi-score.png");
	

	Sprite textures[9];
	for (int j = 0; j < 9; j++)
		textures[j].setTexture(texture[j]);

	//background for the start menu
	textures[0].setScale(1, 1);
	//dino//
	textures[1].setPosition(70, 780);
	textures[1].setScale(0.3, 0.3);
	//cactus//
	textures[2].setPosition(550, 805);
	textures[2].setScale(3, 3);
	//birds//
	textures[3].setPosition(850, 510);
	textures[3].setScale(1.5, 1.5);
	textures[3].setTextureRect(IntRect(94, 0, 188, 80));
	//panel for the instructions
	textures[4].setPosition(1180, 90);
	textures[4].setScale(0.85, 0.85);
	//panel for settings
	textures[5].setPosition(150, 90);
	textures[5].setScale(0.85, 0.85);
	//replay
	textures[6].setPosition(530, 490);
	textures[6].setScale(1.4, 1.4);
	//game over
	textures[7].setPosition(385, 250);
	textures[7].setScale(2, 2);
	//hi score
	textures[8].setPosition(920, 5);
	textures[8].setScale(0.5, 0.5);


	//=================sound system============================
	/*
	  sound buffers order
	  1- Start Menu Music
	  2- Start Game Button Click sound
	  3- Other buttons Click sound
	  4- Jump sound
	  5- Each 100 points reach sound
	  6- Death sound
	  7- Pause Menu Music
	*/

	//Sounds
//Check if can't open any audio file
	SoundBuffer buffer[7];
	if (!buffer[0].loadFromFile("Start Menu Music.mp3")
		|| !buffer[1].loadFromFile("Game Start Button Click Sound.mp3")
		|| !buffer[2].loadFromFile("Other Buttons Click sound.mp3")
		|| !buffer[3].loadFromFile("Jump sound.mp3")
		|| !buffer[4].loadFromFile("100 Points reach sound.mp3")
		|| !buffer[5].loadFromFile("Die sound.mp3")
		|| !buffer[6].loadFromFile("Pause Menu Music.mp3")
		)
		cerr << "Cannot Open File from path specified!!";

	Sound sound[7];
	for (int i = 0; i < 7; i++)
		sound[i].setBuffer(buffer[i]);

	// volume for the background sounds 
	sound[0].setVolume(50.0f);
	sound[6].setVolume(50.0f);

	// volume for the buttons sounds 
	sound[1].setVolume(150.0f);
	
	sound[2].setVolume(20.0f);

	sound[0].play(); //Start Menu Music playing at the back ground
	sound[0].setLoop(true);

	//============Flie Reader Configurations====================
	ifstream reader("score.txt");
	string line;
	reader >> highscore;
	reader.close();

	//anti alising and vysnc
	ContextSettings settings;
	settings.antialiasingLevel = alising;
	RenderWindow window(VideoMode(1920, 1080), "Google Dinosaur!!", Style::Fullscreen, settings);
	int screen_width = window.getSize().x, screen_height = window.getSize().y;
	window.setVerticalSyncEnabled(boxVys);
	window.setFramerateLimit(144);

	//===========System Variables============================
	Time deltatime;
	Clock clock;
	srand(time(nullptr));

	//============Dinosaur Properties=========================
	Texture Dinotexture;
	Dinotexture.loadFromFile("dainosaur movement.png");
	Sprite Dinosaur;
	Dinosaur.setTexture(Dinotexture);
	Dinosaur.setTextureRect(IntRect(0, 0, 98, 92));
	Dinosaur.setPosition(70, ground_level);
	Dinosaur.setScale(1.5, 1.5);

	RectangleShape collision[2];
	collision[0].setSize(Vector2f(70, 50));// Body Collision
	collision[1].setSize(Vector2f(60, 45));// Head Collision


	collision[0].setOrigin(Vector2f(collision[0].getSize().x / 2, collision[0].getSize().y / 2));
	collision[1].setOrigin(Vector2f(collision[1].getSize().x / 2, collision[1].getSize().y / 2));

	collision[0].setFillColor(Color::Green);
	collision[1].setFillColor(Color::Red);

	//============ Ground Properties======================
	Texture groundTEX;
	groundTEX.loadFromFile("Background.png");
	Sprite ground1, ground2;
	ground1.setTexture(groundTEX);
	ground1.setScale(1, 1);
	ground2.setTexture(groundTEX);
	ground2.setScale(1.3, 1);
	ground2.setPosition(ground1.getGlobalBounds().width, 0); // Position second ground sprite right after the first one

	//=================obstacle==========================
	spawned  obstacles;
	obstacles.obs_texture[0].loadFromFile("cactus1.png");
	obstacles.obs_texture[1].loadFromFile("cactus2.png");
	obstacles.obs_texture[2].loadFromFile("cactus3.png");
	obstacles.obs_texture[3].loadFromFile("cactus4.png");
	obstacles.obs_texture[4].loadFromFile("birds.png");


	Sprite bird;
	bird.setTexture(obstacles.obs_texture[4]);
	bird.setScale(Vector2f(1.3, 1.3));
	bool bird_spawned = false;
	Vector2f bird_spawn_locations(ground_level, ground_level - 50);

	//Settings Streaming
	ifstream read_settings("settings.txt");
	read_settings >> boxAlis >> boxVys;

	//Game Body
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (game_state == 1)
			{
				// Pausing the game after pressing Escape Key
				if (event.KeyPressed && event.key.code == Keyboard::Escape)
					isPaused = true;

				if (event.KeyPressed && (event.key.code == Keyboard::Space || event.key.code == Keyboard::Up) && Dinosaur.getPosition().y >= ground_level && !is_bending && !is_dead)
				{
					velocity.y = jump_velocity;
					sound[3].play();
				}

				if (event.KeyPressed && event.key.code == Keyboard::Space && is_dead == true || event.type == Event::MouseButtonPressed && is_dead == true && isMouseOver(window,menuboxes,6))
				{
					score = 0; // Reset score

					// Reset Dinosaur position
					Dinosaur.setPosition(70, ground_level);

					// Clearing obstacles
					obstacles.obs_sprite.clear();
					bird_spawned = false;
					is_dead = false;
					current_frame = 0;
					spawn_counter = 0;
					dino_timer = 0;
					timer = 0;
					bool is_bending = false;
					velocity.x = 450;
				}
				// quit at game over
				if (is_dead && isMouseOver(window, menuboxes, 7) && event.type == Event::MouseButtonPressed) {
					sound[2].play();
					sound[0].pause(); // the music for the start menu will stop;
					ofstream save_settings("settings.txt");
					if (boxAlis)
						save_settings << "1" << endl;
					else
						save_settings << "0" << endl;

					if (boxVys)
						save_settings << "1" << endl;
					else
						save_settings << "0" << endl;
					save_settings.close();

					if (score > highscore)
					{
						highscore = score;
						ofstream writer("score.txt");
						writer << highscore; //overwriting the value of the current socre to be the high score
						writer.close();
					}

					window.close();
				}

			}


			//start menu
			if (game_state == 0)
			{

				menuMouse(menuboxes, selectedOption, window, check, 0);
				menufun(selectedOption, event);

				if ((event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && !isSettingsOpen) || (check && event.type == Event::MouseButtonPressed && !isSettingsOpen))
				{
					if (selectedOption == 0)
					{
						sound[0].pause(); // the music for the start menu will stop;
						sound[1].play(); // music for start button click
						SettingsCheck(boxAlis, boxVys, settingsBoxes, window);
						if (game_starter % 60 == 0)
							game_state = 1;  // the game started playing
						else
							game_starter++;
					}
					//settings
					if (selectedOption == 1)
					{
						sound[2].play();
						isSettingsOpen = true;
						SettingsCheck(boxAlis, boxVys, settingsBoxes, window);
					}

					//Quit
					if (selectedOption == 2) {
						sound[2].play();
						sound[0].pause(); // the music for the start menu will stop;
						ofstream save_settings("settings.txt");
						if (boxAlis)
							save_settings << "1" << endl;
						else
							save_settings << "0" << endl;

						if (boxVys)
							save_settings << "1" << endl;
						else
							save_settings << "0" << endl;
						save_settings.close();

						if (score > highscore)
						{
							highscore = score;
							ofstream writer("score.txt");
							writer << highscore; //overwriting the value of the current socre to be the high score
							writer.close();
						}

						window.close();
					}
				}
				//checking on settings options
				if (isSettingsOpen)
				{
					settingsFun(window, settingsBoxes, event, sound[2]);
				}
			}

			//pause menu
			if(game_state==1)
			if (isPaused && is_dead==false)
			{
				if (isPauseMenuShown == false)
				{
					sound[6].play();
					sound[6].setLoop(true);
					isPauseMenuShown = true;
				}
				menuMouse(menuboxes, selectedOption, window, check, 3);
				menufun(selectedOption, event);
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && !isSettingsOpen || check && event.type == Event::MouseButtonPressed && !isSettingsOpen)
				{
					if (selectedOption == 0)
					{
						isPaused = false;
						sound[2].play();
						sound[6].stop();
						isPauseMenuShown = false;
					}
					//settings
					if (selectedOption == 1)
					{
						SettingsCheck(boxAlis, boxVys, settingsBoxes, window);
						sound[2].play();
						isSettingsOpen = true;
					}

					//quit
					if (selectedOption == 2)
					{
						sound[2].play();
						window.close();

						ofstream save_settings("settings.txt");
						if (boxAlis)
							save_settings << "1" << endl;
						else
							save_settings << "0" << endl;

						if (boxVys)
							save_settings << "1" << endl;
						else
							save_settings << "0" << endl;
						save_settings.close();

						if (score > highscore)
						{
							highscore = score;
							ofstream writer("score.txt");
							writer << highscore; //overwriting the value of the current socre to be the high score
							writer.close();
						}
					}
				}

				if (isSettingsOpen)
					settingsFun(window, settingsBoxes, event, sound[2]);
			}
		}

		//When the game is playing
		if (game_state == 1 && isPaused == false && is_dead == false)
		{

			//bending dinosaur when down arrow key is pressed
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{

				is_bending = true;
				if (dino_timer < 0)
				{
					if (Dinosaur.getPosition().y < ground_level)
						gravity_factor *= 8;
					Dinosaur.setTextureRect(IntRect(480 + (x_daino2 * 124), 0, 124, 92));
					x_daino2 = (x_daino2 + 1) % 2;
					dino_timer = delay;
					collision[0].setPosition(Vector2f(Dinosaur.getPosition().x + 82, Dinosaur.getPosition().y + 88));
					collision[0].setRotation(0);
					collision[1].setPosition(Vector2f(Dinosaur.getPosition().x + 148, Dinosaur.getPosition().y + 78));
				}
				else
					dino_timer -= deltatime.asSeconds();
				collision[0].setPosition(Vector2f(Dinosaur.getPosition().x + 82, Dinosaur.getPosition().y + 88));
				collision[0].setRotation(0);
				collision[1].setPosition(Vector2f(Dinosaur.getPosition().x + 148, Dinosaur.getPosition().y + 78));
			}
			else
			{
				//  Dinosaur movement 
				is_bending = false;
				if (dino_timer < 0)
				{
					Dinosaur.setTextureRect(IntRect(x_daino1 * 98, 0, 96, 92));
					x_daino1 = 1 + x_daino1 % 2;
					dino_timer = delay;
				}
				else
					dino_timer -= deltatime.asSeconds();
				collision[0].setPosition(Vector2f(Dinosaur.getPosition().x + 62, Dinosaur.getPosition().y + 82));
				collision[0].setRotation(-45);
				collision[1].setPosition(Vector2f(Dinosaur.getPosition().x + 98, Dinosaur.getPosition().y + 28));
			}

			//moving Dinosaur to ground and getting affected by gravity
			if (Dinosaur.getPosition().y < ground_level)
			{
				velocity.y += (gravity * gravity_factor) * deltatime.asSeconds();
				gravity_factor += 0.1;
			}

			//moving background continuosly and increasing speed overtime
			ground1.move(-velocity.x * deltatime.asSeconds(), 0);
			ground2.move(-velocity.x * deltatime.asSeconds(), 0);

			if (score <= 1500)
				velocity.x += 0.15f;

			//moving Dinosaur downward due to gravity effect
			if (game_state == 1 && isPaused == false)
				Dinosaur.move(0, velocity.y * deltatime.asSeconds());

			//stopping Dinosaur movement and graivty effect once intersecting the ground
			if (Dinosaur.getPosition().y >= ground_level)
			{
				velocity.y = 0;
				gravity_factor = 1;
				Dinosaur.setPosition(Vector2f(Dinosaur.getPosition().x, ground_level));
			}

			// Move the first ground sprite to the right of the second one
			if (ground1.getPosition().x + ground1.getGlobalBounds().width <= 0)
				ground1.setPosition(ground2.getPosition().x + ground2.getGlobalBounds().width, 0);
			// Check if second ground sprite is completely out of view
			if (ground2.getPosition().x + ground2.getGlobalBounds().width <= 0)
				ground2.setPosition(ground1.getPosition().x + ground1.getGlobalBounds().width, 0);


			current_frame++;//we add this counter each frame to be able to calculate the rate of addition of score

			//adding score by 1 if the current frame accepts the division on 10
			if (current_frame % 18 == 0)
				score++;

			//Playing 100 points reach sound
			if (score % 100 == 0 && current_frame % 18 == 0 && score > 0)
				sound[4].play();

			//zeroing the current frame variable to prevent overflow
			if (current_frame == 144)
				current_frame = 0;


			spawn_counter++;

			//adds new element to the vector when the counter accepts the division by 70;
			if (spawn_counter % spawn_rand == 0 && game_state == 1 && isPaused == false)
			{
				spawn_counter = 0;
				spawn_rand = rand() % 200 + 120;

				random_selector = rand() % 10 + 1;
				int random_texture = rand() % 4;
				if (random_selector == 0 || random_selector == 4 || random_selector == 8)
				{
					if (bird_spawned == false)
					{
						int bird_spawn_selector = rand() % 2;
						bird_spawned = true;
						if (bird_spawn_selector == 0)
							bird.setPosition(Vector2f(screen_width + 100, bird_spawn_locations.x - 60));
						else
							bird.setPosition(Vector2f(screen_width + 100, bird_spawn_locations.y + 80));
					}
					else
					{
						obstacles.obs_sprite.push_back(Sprite());
						obstacles.obs_sprite.back().setTexture(obstacles.obs_texture[random_texture]);
						obstacles.obs_sprite.back().setPosition(Vector2f(screen_width + 100, ground_level + 40));
						obstacles.obs_sprite.back().setScale(1.8, 1.8);

					}
				}
				else
				{
					obstacles.obs_sprite.push_back(Sprite());
					obstacles.obs_sprite.back().setTexture(obstacles.obs_texture[random_texture]);
					obstacles.obs_sprite.back().setPosition(Vector2f(screen_width + 100, ground_level + 40));
					obstacles.obs_sprite.back().setScale(1.8, 1.8);
				}
			}

			//bird movement
			if (bird_spawned == true && game_state == 1 && isPaused == false)
			{

				if (collision[0].getGlobalBounds().intersects(bird.getGlobalBounds()) ||
					collision[1].getGlobalBounds().intersects(bird.getGlobalBounds()))
				{
					sound[5].play();
					is_dead = true;
					velocity.x = 0;
				}

				bird.move(-velocity.x * deltatime.asSeconds(), 0);

				if (timer < 0)
				{
					bird.setTextureRect(IntRect(bird_index * 94, 0, 94, 80));
					bird_index = (bird_index + 1) % 2;
					timer = delay;
				}
				else
					timer -= deltatime.asSeconds();
			}
			//moving all the elements in the vector(moving all the obstacles based on the background speed)
			if (game_state == 1 && isPaused == false)
				for (int i = 0; i < obstacles.obs_sprite.size(); i++)
				{
					obstacles.obs_sprite[i].move(-velocity.x * deltatime.asSeconds(), 0);

					//sopping Dinosaur movement when intersecting with obstacles
					if (collision[0].getGlobalBounds().intersects(obstacles.obs_sprite[i].getGlobalBounds()) ||
						collision[1].getGlobalBounds().intersects(obstacles.obs_sprite[i].getGlobalBounds()))
					{
						sound[5].play();
						velocity.x = 0;
						is_dead = true;
					}
				}

			scores[0].setString(to_string(score));
			scores[1].setString(to_string(highscore));

			deltatime = clock.restart();

			for (int i = 0; i < obstacles.obs_sprite.size(); i++)
				if (obstacles.obs_sprite[i].getPosition().x < -100)
					obstacles.obs_sprite.erase(obstacles.obs_sprite.begin());

			if (bird.getPosition().x < -100)
				bird_spawned = false;
		}
		// Elemnts Draw section
		if (game_state == 1)
		{
			window.clear(Color::Black);

			window.draw(ground1);
			window.draw(ground2);
			for (int i = 0; i < obstacles.obs_sprite.size(); i++)
				window.draw(obstacles.obs_sprite[i]);//drawing all the vector elements
			if (bird_spawned == true)
				window.draw(bird);
			window.draw(Dinosaur);
			// draw score and high score while the game is playing
			window.draw(scores[0]);
			window.draw(scores[1]);
			window.draw(textures[8]); // HI 
			//game over
			if (is_dead) {

				// textures for game over
				for (int i = 6; i < 9; i++)
					window.draw(textures[i]);
				// draw quit text 
				window.draw(menu[6]);

				//draw boxes for the game over
				window.draw(menuboxes[6]); 
				window.draw(menuboxes[7]);
				//draw score and highscore at game over
				window.draw(scores[0]);
				window.draw(scores[1]);
			}
			if (isPaused && isSettingsOpen && is_dead==false)
			{
				window.draw(textures[5]);

				for (int i = 0; i < 7; i++)
					window.draw(settingsBoxes[i]);

				for (int i = 0; i < 5; i++)
					window.draw(settingsText[i]);

				
				// draw instructions at pause menu
				window.draw(textures[4]);
				window.draw(paneltext);
				window.draw(insttext);
				window.draw(instructionstext);
				window.draw(titletext);
				window.draw(Instructions_title);

				checkOption(selectedOption, menu, 3, 4, 5);  //hover effect for the puse menu
			}

			

			//draw pause menu
			else if (isPaused && is_dead==false)
			{

				for (int i = 3; i < 6; i++)
				{
					window.draw(menuboxes[i]);
					window.draw(menu[i]);

				}
				checkOption(selectedOption, menu, 3, 4, 5);  //hover effect for the puse menu
			}
		}

		if (game_state == 0)
		{
			for (int i = 0; i < 5; i++)
				window.draw(textures[i]);

			for (int i = 0; i < 3; i++)
				window.draw(menuboxes[i]);

			if (!isSettingsOpen)
				for (int i = 0; i < 3; i++)
					window.draw(menu[i]);

			window.draw(paneltext);
			window.draw(insttext);
			window.draw(instructionstext);
			window.draw(titletext);
			window.draw(Instructions_title);

			if (isSettingsOpen)
			{
				window.draw(textures[5]);

				for (int i = 0; i < 7; i++)
					window.draw(settingsBoxes[i]);

				for (int i = 0; i < 5; i++)
					window.draw(settingsText[i]);
			}
			checkOption(selectedOption, menu, 0, 1, 2);  //hover effect for the start menu
		}
		window.display();
	} //while loop
}// main