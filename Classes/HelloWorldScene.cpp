/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    draw = DrawNode::create();
    this->addChild(draw);
    
    firstColor = Color3B::RED;
    secondColor = Color3B::YELLOW;
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    this->scheduleUpdate();

    
    return true;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    
}

void HelloWorld::update(float dt) {
    time += dt;
    if (time > 0.05f) {
        time -= 0.05f;
        if ((int)path.size() > 1) {
            path.erase(path.end() - 1);
        }
    }
    
    drawSlash();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {
    addPoint(touch->getLocation());
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event) {
    addPoint(touch->getLocation());
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
    draw->clear();
    path.clear();
}

void HelloWorld::drawSlash() {
    draw->clear();
    if ((int)path.size() < 2) return;
    
    float size = 10;
    float addSize = 10.0f / (float)path.size();
    
    Color3B curColor = secondColor;
    Color3B addColor = Color3B(
                               (firstColor.r - secondColor.r)/(int)path.size(),
                               (firstColor.g - secondColor.g)/(int)path.size(),
                               (firstColor.b - secondColor.b)/(int)path.size()
                        );
    
    std::vector<Vec2> top, bot;
    unsigned int i = 0;
    for (Vec2 v : path)
    {
        Vec2 normalTop;
        
        Vec2 perpendicularL = Vec2::ZERO;
        Vec2 perpendicularR = Vec2::ZERO;
        
        if (i < (int)path.size() - 1) {
            cocos2d::Vec2 dir = path[i + 1] - path[i];
            perpendicularR.set(-dir.y, dir.x);
            perpendicularR.normalize();
        }
        
        if (i > 0) {
            cocos2d::Vec2 dir2 = path[i] - path[i - 1];
            perpendicularL.set(-dir2.y, dir2.x);
            perpendicularL.normalize();
        }
        
        normalTop = perpendicularL + perpendicularR;
        normalTop.normalize();
        
        top.push_back(v + (normalTop * size));
        bot.push_back(v - (normalTop * size));
        
        size -= addSize;
        i++;
    }
    
    for (int i=0; i<(int)top.size() - 1; i++) {
        std::vector<Vec2> poly;
        poly.push_back(bot.at(i));
        poly.push_back(top.at(i));
        poly.push_back(top.at(i + 1));
        poly.push_back(bot.at(i + 1));
        
        draw->drawSolidPoly(poly.data(), (int)poly.size(), Color4F(curColor));
        
        curColor = Color3B(curColor.r + addColor.r, curColor.g + addColor.g, curColor.b + addColor.b);
    }
    
    std::vector<Vec2> poly2;
    Vec2 delta = path[1] - path[0];
    delta.normalize();
    
    poly2.push_back(path[0] - delta * 20);
    poly2.push_back(top[0]);
    poly2.push_back(bot[0]);
    draw->drawSolidPoly(poly2.data(), (int)poly2.size(), Color4F(secondColor));
}

void HelloWorld::addPoint(Vec2 point) {
    path.insert(path.begin(), point);
    
    if ((int)path.size() > 20) {
        path.erase(path.end() - 1);
    }
}
