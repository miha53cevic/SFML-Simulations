#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

class mihaSimpleSFML
{
public:
	mihaSimpleSFML()
	{
		m_sAppName = L"Default";
		m_nScreenWidth = 640;
		m_nScreenHeight = 480;

		m_bVSync = false;
		m_bDone = false;

		m_backgroundColor = sf::Color::Black;

		//Loading Font
		if (m_font.loadFromFile("consola.ttf")) {}
		m_fpsCounter.setFont(m_font);

		m_bShowFps = false;
		m_window.setKeyRepeatEnabled(false);
	}
	~mihaSimpleSFML() { m_window.close(); }

	//Must override these
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(sf::Time elapsed) = 0;

	int ScreenWidth() { return m_nScreenWidth; }
	int ScreenHeight() { return m_nScreenHeight; }

	void EnableVSync(bool l_vsync) { m_bVSync = l_vsync; m_window.setVerticalSyncEnabled(m_bVSync); }
	bool IsVSyncOn() { return m_bVSync; }
	
	void ShowFps(bool l_showFps) { m_bShowFps = l_showFps; }
	
	sf::RenderWindow* getWindow() { return &m_window; }

	void Draw(sf::Drawable& l_drawable) { m_window.draw(l_drawable); }
	void Draw(sf::Drawable& l_drawable, sf::Transform& l_transform) { m_window.draw(l_drawable, l_transform); }

	void setBackgroundColor(sf::Color l_color) { m_backgroundColor = l_color; }

private:
	sf::RenderWindow m_window;

	int m_nScreenWidth;
	int m_nScreenHeight;

	std::wstring m_sAppName;

	sf::Color m_backgroundColor;

	bool m_bVSync;

protected:
	bool m_bDone;

private:
	sf::Clock m_clock;
	sf::Time m_elapsed;

	void BeginDraw(sf::Color l_color) { m_window.clear(l_color); }
	void EndDraw() { m_window.display(); }

	//FPS Display
	sf::Text m_fpsCounter;
	sf::Font m_font;

	bool m_bShowFps;
	float m_fps;

public:
	void Construct(int l_nScreenWidth, int l_nScreenHeight, std::wstring l_title)
	{
		m_nScreenWidth = l_nScreenWidth;
		m_nScreenHeight = l_nScreenHeight;

		m_sAppName = l_title;
	}

	void Start()
	{
		m_window.create(sf::VideoMode(m_nScreenWidth, m_nScreenHeight), m_sAppName);

		if (!OnUserCreate())
			m_bDone = true;

		//Main Game Loop
		while (!m_bDone)
		{
			BeginDraw(m_backgroundColor);

			if (!OnUserUpdate(m_elapsed))
				m_bDone = true;

			if (m_bShowFps)
			{
				m_fps = 1.0f / m_elapsed.asSeconds();
				m_fpsCounter.setString(std::to_string(m_fps));
				m_window.draw(m_fpsCounter);
			}
			
			EndDraw();

			m_elapsed = m_clock.restart();
		}
	}
};