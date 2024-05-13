//SANTIAGO EXEQUIEL FERNÁNDEZ - MAVI II, U3 - Ejercicio 5:

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"
#include "Box2dEmptyApp.h"
#include <tchar.h>

using namespace sf;

Ragdoll::Ragdoll(b2World* world, sf::RenderWindow* window, float x, float y)
    : wnd(window), _body(nullptr)
{
    // TORSO:
    b2Body* torso = CreateBodyPart(world, x, y, 8.0f, 12.0f);

    // CABEZA:
    b2Body* cabeza = CreateBodyPart(world, x, y - 10.0f, 6.0f, 6.0f);

    // BRAZOS:
    b2Body* brazoIzq = CreateBodyPart(world, x - 6.0f, y + 3.0f, 4.0f, 6.0f);
    b2Body* brazoDer = CreateBodyPart(world, x + 6.0f, y + 3.0f, 4.0f, 6.0f);

    // PIERNAS:
    b2Body* piernaIzq = CreateBodyPart(world, x - 3.0f, y + 30.0f, 4.0f, 10.0f);
    b2Body* piernaDer = CreateBodyPart(world, x + 3.0f, y + 30.0f, 4.0f, 10.0f);

    // Uniones con revolute joints:
    CreateSpringJoint(world, cabeza, torso, b2Vec2(0.0f, 1.0f), b2Vec2(0.0f, -8.0f), -0.2f * b2_pi, 0.2f * b2_pi);
    CreateSpringJoint(world, brazoIzq, torso, b2Vec2(-5.0f, 0.0f), b2Vec2(0.0f, -4.0f), -0.6f * b2_pi, 0.6f * b2_pi);
    CreateSpringJoint(world, brazoDer, torso, b2Vec2(5.0f, 0.0f), b2Vec2(0.0f, -4.0f), -0.6f * b2_pi, 0.6f * b2_pi);
    CreateSpringJoint(world, piernaIzq, torso, b2Vec2(-4.0f, 0.0f), b2Vec2(0.0f, 4.0f), -0.4f * b2_pi, 0.4f * b2_pi);
    CreateSpringJoint(world, piernaDer, torso, b2Vec2(4.0f, 0.0f), b2Vec2(0.0f, 4.0f), -0.4f * b2_pi, 0.4f * b2_pi);
}

// Método para crear un resorte entre dos cuerpos con restricciones de rotación:
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

void Ragdoll::Despertar()
{
    _body->SetAwake(true);
}

void Ragdoll::ApplyLinearImpulse(const b2Vec2& impulse, const b2Vec2& point)
{
    // Aplica un impulso lineal al cuerpo principal del ragdoll
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
