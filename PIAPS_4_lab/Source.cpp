#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;


class Image {
public:
	virtual void draw(sf::RenderWindow& window) = 0;

protected:
	sf::Texture textureBox;
	sf::Sprite spriteBox; // спрайт изображени€
	sf::Vector2f boxPos = { 800, 900 };
	bool hasImage = false; // флаг, указывающий, загружено ли изображение в бокс
	sf::Vector2f mousePos;
	bool isDragging = false;

	void handleDragging(sf::Event& event, sf::RenderWindow& window, sf::Sprite& sprite, sf::Texture& texture) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			// —охран€ем координаты нажати€ мыши и позицию пр€моугольника
			mousePos = { static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
			isDragging = sprite.getGlobalBounds().contains(mousePos);
			boxPos = sprite.getPosition();
			sprite.setColor(sf::Color(175, 255, 255, 175));
		}
		else if (event.type == sf::Event::MouseMoved && isDragging) {
			// ¬ычисл€ем новую позицию пр€моугольника в соответствии с перемещением мыши
			sf::Vector2f offset = { static_cast<float>(event.mouseMove.x) - mousePos.x,
								   static_cast<float>(event.mouseMove.y) - mousePos.y };
			sf::Vector2f newPos = boxPos + offset;

			// ѕровер€ем, не выходит ли пр€моугольник за границы окна
			if (newPos.x < 0) newPos.x = 0;
			if (newPos.y < 0) newPos.y = 0;
			if (newPos.x + texture.getSize().x > window.getSize().x)
				newPos.x = window.getSize().x - texture.getSize().x;
			if (newPos.y + texture.getSize().y > window.getSize().y)
				newPos.y = window.getSize().y - texture.getSize().y;

			sprite.setPosition(newPos);
		}
		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			// —брасываем флаг перетаскивани€
			sprite.setColor(sf::Color::White);
			isDragging = false;
		}
	}
};



class RealImage : public Image {
public:
	RealImage() {}

	void draw(sf::RenderWindow& window) override {
		
	
		if (textureBox.loadFromFile("TestImage.jpg")) // загрузка изображени€ с диска, если еще не загружено
		{
			hasImage = true;
			spriteBox.setTexture(textureBox);
			spriteBox.setPosition(window.getSize().x / 2 - textureBox.getSize().x / 2, window.getSize().y / 2 - textureBox.getSize().y / 2);
			

		}
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) 
			{
				if (event.type == sf::Event::Closed) 
				{
					window.close();
				}
				else {
					handleDragging(event, window, spriteBox, textureBox);
				}
			}

			window.clear();
			if (hasImage) 
			{
				window.draw(spriteBox);

			}
			window.display();
		}
	}
};


class ImageProxy : public Image {
public:
	
	ImageProxy() {}

	ImageProxy(sf::RenderWindow& window) {}

	void draw(sf::RenderWindow& window) override 
	
	{
		/*sf::Texture textureBox;*/
		if (!textureBox.loadFromFile("box.jpg")) 
		{
			cout << "Not file box.jpg!!!" << endl;
		}

		sf::Sprite spriteBox(textureBox);
		spriteBox.setPosition(window.getSize().x / 2 - textureBox.getSize().x / 2,
			window.getSize().y / 2 - textureBox.getSize().y / 2);

		sf::Clock doubleClickClock;
		const float doubleClickTimeThreshold = 0.5f; // in seconds
		bool rightButtonPressed = false;

		while (window.isOpen()) 
		{
			sf::Event event;
			while (window.pollEvent(event)) 
			{
				if (event.type == sf::Event::Closed) 
				{
					window.close();
				}
				else
				{
					handleDragging(event, window, spriteBox, textureBox);
					
				 if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
					{
						if (rightButtonPressed && doubleClickClock.getElapsedTime().asSeconds() < doubleClickTimeThreshold)
						{
							
							rightButtonPressed = false;
							

							if (!hasImage)
							{
								textureBox = sf::Texture();
								spriteBox = sf::Sprite();
								RealImage* realImage = new RealImage;
								realImage->draw(window);
							}

						}
							
							else
								{
									rightButtonPressed = true;
									doubleClickClock.restart();
								}
					
					}
				}

			}

			window.clear();
			window.draw(spriteBox);
			window.display();
			
		}

	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 1000), "Simple Graphical Editor");
	ImageProxy* imageProxy = new ImageProxy;
	imageProxy->draw(window);
	return 0;
}
