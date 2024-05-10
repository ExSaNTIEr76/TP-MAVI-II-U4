//SANTIAGO EXEQUIEL FERNÁNDEZ - MAVI II, U3 - Ejercicio 5:

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2Body* controlBody;
	b2Body* draggedBody;
	b2Vec2 clickOffset;
	b2World* phyWorld;
	SFMLRenderer* debugRender;
	sf::RectangleShape bocaCanon;

	// Tiempo de frame
	float frameTime;
	int fps;

public:
	//Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
	void CreateRagdollFromCannon();
};