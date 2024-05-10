//SANTIAGO EXEQUIEL FERNÁNDEZ - MAVI II, U3 - Ejercicio 5:

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"
#include "Box2dEmptyApp.h"
#include <tchar.h>

using namespace sf;

// RAGDOLL:
//Ragdoll* ragdoll = new Ragdoll(phyWorld, wnd, 50.0f, 50.0f);

Ragdoll::Ragdoll(b2World* world, sf::RenderWindow* window, float x, float y)
    : wnd(window), _body(nullptr)
{
    // Creamos el torso
    b2Body* torso = CreateBodyPart(world, x, y, 8.0f, 12.0f);

    // Creamos la cabeza
    b2Body* cabeza = CreateBodyPart(world, x, y - 10.0f, 6.0f, 6.0f); // Posición de la cabeza ajustada para estar más cerca del torso

    // Creamos los brazos
    b2Body* brazoIzq = CreateBodyPart(world, x - 6.0f, y + 3.0f, 4.0f, 6.0f); // Posición del brazo izquierdo ajustada para estar más baja
    b2Body* brazoDer = CreateBodyPart(world, x + 6.0f, y + 3.0f, 4.0f, 6.0f); // Posición del brazo derecho ajustada para estar más baja

    // Creamos las piernas
    b2Body* piernaIzq = CreateBodyPart(world, x - 3.0f, y + 30.0f, 4.0f, 10.0f); // Posición de la pierna izquierda ajustada para estar más abajo
    b2Body* piernaDer = CreateBodyPart(world, x + 3.0f, y + 30.0f, 4.0f, 10.0f); // Posición de la pierna derecha ajustada para estar más abajo

    // Unimos las partes del cuerpo con revolute joints
    CreateSpringJoint(world, cabeza, torso, b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, -8.0f), -0.2f * b2_pi, 0.2f * b2_pi); // Unión cabeza - torso
    CreateSpringJoint(world, brazoIzq, torso, b2Vec2(-5.0f, 0.0f), b2Vec2(0.0f, -4.0f), -0.6f * b2_pi, 0.6f * b2_pi); // Unión brazo izquierdo - torso
    CreateSpringJoint(world, brazoDer, torso, b2Vec2(5.0f, 0.0f), b2Vec2(0.0f, -4.0f), -0.6f * b2_pi, 0.6f * b2_pi); // Unión brazo derecho - torso
    CreateSpringJoint(world, piernaIzq, torso, b2Vec2(-4.0f, 0.0f), b2Vec2(0.0f, 4.0f), -0.4f * b2_pi, 0.4f * b2_pi); // Unión pierna izquierda - torso
    CreateSpringJoint(world, piernaDer, torso, b2Vec2(4.0f, 0.0f), b2Vec2(0.0f, 4.0f), -0.4f * b2_pi, 0.4f * b2_pi); // Unión pierna derecha - torso
}

// Método para crear un resorte entre dos cuerpos con restricciones de rotación
void Ragdoll::CreateSpringJoint(b2World* world, b2Body* bodyA, b2Body* bodyB, b2Vec2 localAnchorA, b2Vec2 localAnchorB, float lowerAngle, float upperAngle)
{
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.bodyB = bodyB;
    jointDef.localAnchorA = localAnchorA;
    jointDef.localAnchorB = localAnchorB;
    jointDef.lowerAngle = lowerAngle;
    jointDef.upperAngle = upperAngle;
    jointDef.enableLimit = true; // Habilitar límites de ángulo
    world->CreateJoint(&jointDef);
}

// Método para actualizar la posición del sprite basada en la posición del cuerpo físico
void Ragdoll::ActualizarPosiciones()
{
    b2Vec2 pos = _body->GetPosition();
    sprite.setPosition(pos.x, pos.y);
}

void Ragdoll::Despertar()
{
    _body->SetAwake(true);
}

// Método para dibujar el sprite en la ventana de renderizado
void Ragdoll::Dibujar()
{
    wnd->draw(sprite);
}


void Ragdoll::ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point)
{
    // Aplicar impulso lineal al cuerpo principal del ragdoll
    if (_body)
    {
        _body->ApplyLinearImpulse(impulse, point, true);
    }
}

b2Body* Ragdoll::CreateBodyPart(b2World* world, float x, float y, float width, float height)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f, height / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    body->CreateFixture(&fixtureDef);

    return body;
}


int _tmain(int argc, _TCHAR* argv[])
{
    // Creación del juego y bucle principal
    Game* Juego;
    Juego = new Game(800, 600, "MAVII II, U3 - Ejercicio 5");
    Juego->Loop(); // Inicia el bucle principal del juego

    return 0;
}
