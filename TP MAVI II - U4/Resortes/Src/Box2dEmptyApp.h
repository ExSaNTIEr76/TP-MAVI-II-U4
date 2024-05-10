#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Ragdoll
{
public:
    Ragdoll(b2World* world, sf::RenderWindow* window, float x, float y);

    // M�todo para actualizar la posici�n del sprite basada en la posici�n del cuerpo f�sico
    void ActualizarPosiciones();

    // M�todo para dibujar el sprite en la ventana de renderizado
    void Dibujar();

    // M�todo para aplicar una fuerza de impulso lineal al cuerpo del ragdoll
    void ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point);

    void Despertar();
    b2Body* _body;
private:
    // Funci�n auxiliar para crear partes del cuerpo del ragdoll
    b2Body* CreateBodyPart(b2World* world, float x, float y, float width, float height);

    // Funci�n auxiliar para crear una uni�n de resorte entre dos cuerpos del ragdoll
    void CreateSpringJoint(b2World* world, b2Body* bodyA, b2Body* bodyB, b2Vec2 localAnchorA, b2Vec2 localAnchorB, float lowerAngle, float upperAngle);

    sf::RenderWindow* wnd; // Ventana de renderizado
    sf::Texture texture;   // Textura del ragdoll
    sf::Sprite sprite;     // Sprite del ragdoll
};

