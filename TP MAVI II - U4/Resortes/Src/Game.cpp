//SANTIAGO EXEQUIEL FERNÁNDEZ - MAVI II, U3 - Ejercicio 5:

#include "Game.h"
#include "Box2DHelper.h"
#include "Box2dEmptyApp.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
	draggedBody = nullptr; // Inicializa draggedBody como nullptr
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		UpdatePhysics();
		DrawGame();
		wnd->display();
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close();
			break;
		case Event::MouseButtonPressed:
			if (evt.mouseButton.button == Mouse::Left)
			{
				// Crear un ragdoll en la posición de la boca del cañón con dirección adecuada
				CreateRagdollFromCannon();
			}
			break;
		}
	}
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar los obstáculos
	sf::RectangleShape obstacle(sf::Vector2f(10, 10));
	obstacle.setFillColor(sf::Color::Cyan);
	obstacle.setPosition(70, 60);
	wnd->draw(obstacle);

	// Dibujar el cuerpo de control (círculo)
	sf::CircleShape controlShape(5);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(controlBody->GetPosition().x - 5, controlBody->GetPosition().y - 5);
	wnd->draw(controlShape);

    sf::RectangleShape pieCanon(sf::Vector2f(8, 10));
    pieCanon.setFillColor(sf::Color::Cyan);
    pieCanon.setPosition(10, 85); // Ajustamos la posición del pie del cañón

    // Dibujar la boca del cañón
    sf::RectangleShape bocaCanon(sf::Vector2f(15, 10));
    bocaCanon.setFillColor(sf::Color::Magenta);
    bocaCanon.setPosition(10, 80);
    bocaCanon.setOrigin(0, bocaCanon.getSize().y / 2.0f);

	// Calcular la posición del cursor en relación con la ventana
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*wnd);
	sf::Vector2f cannonMouthPosition = bocaCanon.getPosition() + sf::Vector2f(bocaCanon.getSize().x / 2.0f, bocaCanon.getSize().y / 2.0f);
	sf::Vector2f cannonDirection = sf::Vector2f(mousePosition.x, mousePosition.y) - cannonMouthPosition;

	// Normalizar el vector de dirección del cañón
	float length = std::sqrt(cannonDirection.x * cannonDirection.x + cannonDirection.y * cannonDirection.y);
	if (length != 0) {
		cannonDirection /= length;
	}

	// Calcular el ángulo entre la boca del cañón y la posición del cursor
	float angle = std::atan2(cannonDirection.y, cannonDirection.x) * 180 / b2_pi;

	// Rotar la boca del cañón
	bocaCanon.setRotation(angle);

	// Dibujar el pie del cañón
	wnd->draw(pieCanon);

	// Dibujar la boca del cañón
	wnd->draw(bocaCanon);
}

b2Vec2 ConvertVector(const sf::Vector2f& vector) {
	return b2Vec2(vector.x, vector.y);
}

void Game::CreateRagdollFromCannon()
{
	// Calcular la posición de la boca del cañón en relación con la ventana
	sf::Vector2f cannonMouthPosition = bocaCanon.getPosition() + sf::Vector2f(25, 60);

	// Calcular la dirección del ragdoll basado en el ángulo de la boca del cañón
	float angleRad = bocaCanon.getRotation() * b2_pi / 180.0f;
	b2Vec2 ragdollDirection(std::cos(angleRad), std::sin(angleRad));

	// Crear un ragdoll en la posición de la boca del cañón
	Ragdoll* ragdoll = new Ragdoll(phyWorld, wnd, cannonMouthPosition.x, cannonMouthPosition.y);
			b2Vec2 b2CannonMouthPosition = ConvertVector(cannonMouthPosition);
	// Verificar si el ragdoll se ha creado correctamente
	if (ragdoll && ragdoll->_body) {
		// Aplicar una fuerza de eyección al ragdoll en la dirección del cursor
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*wnd);
		sf::Vector2f direction = sf::Vector2f(mousePosition.x, mousePosition.y) - cannonMouthPosition;
		direction = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y); // Normalizar el vector
		b2Vec2 impulse(10.0f, 0.0f); // Vector de impulso (en Newtons)
		b2Vec2 b2CannonMouthPosition = ConvertVector(cannonMouthPosition);

		// Aplicar una fuerza de eyección al ragdoll en la dirección del cursor
		ragdoll->ApplyLinearImpulse(impulse, b2CannonMouthPosition);

		// Despertar al ragdoll después de aplicar la fuerza
		ragdoll->Despertar();
	}
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 80.0f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes
	// También crea cuerpos dinámicos (como círculos) y los une con resortes al techo para demostrar la simulación física
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	b2Body* obstacle = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 10);
	obstacle->SetTransform(b2Vec2(75.0f, 65.0f), 0.0f);

	controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 10.0f, 5.0, 0.0f);
	controlBody->SetTransform(b2Vec2(75.0f, 60.0f), 0.0f);

	//CAÑÓN:
	b2Body* pieCanon = Box2DHelper::CreateRectangularStaticBody(phyWorld, 8, 10);
	pieCanon->SetTransform(b2Vec2(14.0f, 90.0f), 0.0f);;

}