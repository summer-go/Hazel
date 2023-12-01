//
// Created by ByteDance on 2023/11/16.
//

#include "Level.h"
#include "Renderer/Renderer2D.h"
#include "Random.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Hazel;
static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
    int H = (int)(hsv.x * 360.0f);
    double S = hsv.y;
    double V = hsv.z;

    double C = S * V;
    double X = C * (1 - abs(fmod(H/60.0,2) -1));
    double m = V - C;
    double Rs = 0, Gs = 0, Bs = 0;

    if (H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if (H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if (H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if (H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if (H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    } else {
        Gs = C;
        Gs = 0;
        Bs = X;
    }
    return {(Rs + m), (Gs + m), (Bs + m), 1.0f};
}

// 计算三角形p p0 p1的面积是否 > p0 p1 p2的面积
static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    // 叉乘计算三角形p0 p2 p的面积和方向
    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    // 叉乘计算三角形p0 p1 p的面积和方向
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;
    // 判断这两个三角形面积的方向是否一致，不一致直接返回false
    if ((s < 0) != (t < 0))
        return false;

    // 叉乘计算三角形p0 p1 p2的面积
    float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

    // 两个三角形面积s t之和 < A时，则点p在三角形内
    // 注意考虑A的方向
    return A < 0 ?
           (s <= 0 && s + t >= A) :
           (s >= 0 && s + t <= A);
}

void Level::Init() {
    m_TriangleTexture = Texture2D::Create("../assets/textures/Triangle.png");
    m_Player.LoadAssets();
    m_Pillars.resize(5);
    for (int i = 0; i < 5; i++) {
        CreatePillar(i, i * 10.0f);
    }
}

void Level::OnUpdate(Hazel::Timestep ts) {
    m_Player.OnUpdate(ts);

    if (CollisionTest()) {
        HZ_INFO("Level::OnUpdate:true");
        GameOver();
        return;
    }

    m_PillarHSV.x += 0.1f * ts;
    if (m_PillarHSV.x > 1.0f) {
        m_PillarHSV.x = 0.0f;
    }

    // 搞清楚这个pillar Target的逻辑
     if (m_Player.GetPosition().x > m_PillarTarget) {
        HZ_INFO("Level::OnUpdate m_PillarIndex = {0}; m_PillarTarget = {1}; m_PillarTarget + 20.0f = {2}",m_PillarIndex, m_PillarTarget, m_PillarTarget + 20.0f);

        CreatePillar(m_PillarIndex, m_PillarTarget + 20.0f);
        m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
        m_PillarTarget += 10.0f;
    }
}

void Level::OnRender() {
    const auto& playerPos = m_Player.GetPosition();

    glm::vec4 color = HSVtoRGB(m_PillarHSV);

    // Background
    Renderer2D::DrawQuad({playerPos.x, 0.0f, -0.8f}, {50.0f, 50.0f}, {0.3f, 0.3f, 0.3f, 1.0f});

    // Floor and ceiling
    Renderer2D::DrawQuad({playerPos.x, 34.0f}, {50.0f, 50.0f}, color);
    Renderer2D::DrawQuad({playerPos.x, -34.0f}, {50.0f, 50.0f}, color);

    for(auto& pillar : m_Pillars)
    {
        Renderer2D::DrawQuad(pillar.TopPosition, pillar.TopScale, glm::radians(180.0f), m_TriangleTexture, 1.0f, color);
        Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, 0.0f, m_TriangleTexture, 1.0f, color);
    }
    m_Player.OnRender();
}


void Level::OnImGuiRender() {
    m_Player.OnImGuiRender();
}


void Level::CreatePillar(int index, float offset) {
    HZ_INFO("Level::CreatePillar index = {0}; offset = {1}",index, offset);

    Pillar& pillar = m_Pillars[index];
    pillar.TopPosition.x = offset;
    pillar.BottomPosition.x = offset;
    pillar.TopPosition.z = index * 0.1f - 0.5f;
    pillar.BottomPosition.z = index * 0.1f - 0.5f + 0.5f;

    float center = Random::Float() * 35.0f - 17.5f;
    float gap = 2.0f + Random::Float() * 5.0f;

    // todo：解释y的计算原理
    pillar.TopPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
    pillar.BottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

bool Level::CollisionTest() {
    if (glm::abs(m_Player.GetPosition().y) > 8.5f){
        return true;
    }

    glm::vec4 playerVertices[4] = {
            {-0.5f, -0.5f, 0.0f, 1.0f},
            {0.5f, -0.5f, 0.0f, 1.0f},
            {0.5f, 0.5f, 0.0f, 1.0f},
            {-0.5f, 0.5f, 0.0f, 1.0f},
    };

    const auto& pos = m_Player.GetPosition();
    glm::vec4 playerTransformedVerts[4];
    for (int i = 0; i < 4; i++) {
        playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f})
                * glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), {0.0f, 0.0f, 1.0f})
                * glm::scale(glm::mat4(1.0f), {1.0f, 1.3f, 1.0f})
                * playerVertices[i];
    }

    // To match  triangle.png (each corner is 10% from the texture edge)
    glm::vec4 pillarVertices[3] = {
            {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
            {0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
            {0.0f + 0.0f, 0.5f - 0.1f, 0.0f, 1.0f},
    };

    for (auto& p : m_Pillars) {
        glm::vec2 tri[3];

        // Top pillars
        for (int i = 0; i < 3; i++) {
            tri[i] = glm::translate(glm::mat4(1.0f), {p.TopPosition.x, p.TopPosition.y, 0.0f})
                    * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 0.0f, 1.0f})
                    * glm::scale(glm::mat4(1.0f), {p.TopScale.x, p.TopScale.y, 1.0f})
                    * pillarVertices[i];
        }

        for (auto& vert : playerTransformedVerts) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])) {
                return true;
            }
        }

        // Bottom pillars
        for (int i = 0; i < 3; i++) {
            tri[i] = glm::translate(glm::mat4(1.0f), {p.BottomPosition.x, p.BottomPosition.y, 0.0f})
                    * glm::scale(glm::mat4(1.0f), {p.BottomScale.x, p.BottomScale.y, 1.0f})
                    * pillarVertices[i];
        }

        for (auto& vert : playerTransformedVerts) {
            if (PointInTri({vert.x, vert.y}, tri[0], tri[1], tri[2])) {
                return true;
            }
        }

    }
    return false;
}

void Level::GameOver() {
    HZ_INFO("Level::GameOver()");
    m_GameOver = true;
}

void Level::Reset() {
    m_GameOver = false;
    m_Player.Reset();

    m_PillarTarget = 30.0f;
    m_PillarIndex = 0;
    for (int i = 0; i < 5; i++) {
        CreatePillar(i, i * 10.0f);
    }
}
