#ifndef TEST_OBJ_H
#define TEST_OBJ_H

#include <QObject>
#include <QTest>

#include <memory>

#include <shapes/gobjcircle.h>
#include <shapes/gobjrect.h>

class TestCircle : public GColliderCircle {
    using GColliderCircle::GColliderCircle;

};
class TestRect : public GColliderRect {
    using GColliderRect::GColliderRect;
};


class Test_GObj : public QObject
{
    Q_OBJECT

    auto getCheckFunc(int r) {
        return [r](const std::shared_ptr<GObj> & i) { //check if the droids we're looking for?
            auto testCircle = std::dynamic_pointer_cast<TestCircle>(i);
            if (testCircle == nullptr)
                return false;
            return testCircle->getR() == r;
        };
    }

    auto findByRR(const GObj::GObjP & gObjContainer, int r) {
        auto gObj = gObjContainer->findChild(getCheckFunc(r));
        return std::dynamic_pointer_cast<TestCircle>(gObj);
    }

    const int N = 10;
    const int R = 10;

private slots:
    void initTestCase() {

    }

    void getPosAbsolute() {
        auto gObjContainer = std::make_shared<GObj>();
        auto gObjSubContainer1 = gObjContainer->addChild<GObj>(std::make_shared<GObj>(GPos({10,0,0})));
        auto gObjSubContainer2 = gObjSubContainer1->addChild<GObj>(std::make_shared<GObj>(GPos({0,10,0})));
        auto testCircle = gObjSubContainer2->addChild<TestCircle>(std::make_shared<TestCircle>(10,GPos({10,10,0})));
        QVERIFY(testCircle->getAPos().getX() == 20);
        QVERIFY(testCircle->getAPos().getY() == 20);
    }

    void isIntersectWith() {
        auto testCircle = std::make_shared<TestCircle>(10, GPos(0,0,0));
        auto testCircle1 = std::make_shared<TestCircle>(10, GPos({10,0,0}));
        auto testCircle2 = std::make_shared<TestCircle>(10, GPos({20,0,0}));
        auto testRect1 = std::make_shared<TestRect>(10,10, GPos({0,0,0}));
        auto testRect2 = std::make_shared<TestRect>(10,10, GPos({0,20,0}));
        QVERIFY(testCircle->isIntersectWith(testCircle1) == true);
        QVERIFY(testCircle->isIntersectWith(testCircle2) == false);
        QVERIFY(testCircle->isIntersectWith(testRect1) == true);
        QVERIFY(testCircle->isIntersectWith(testRect2) == false);
    }

    void findIntersectObjs() {
        auto root = std::make_shared<GObj>();
        auto mainCircle = root->ADD_CHILD(TestCircle, 10, GPos({0,0,0}));
        auto intersect1 = root->ADD_CHILD(TestCircle, 5, GPos({14,0,0}));
        auto intersect2 = root->ADD_CHILD(TestCircle, 15, GPos({0,24,0}));
        auto intersectNo = root->ADD_CHILD(TestCircle, 15, GPos({0,25,0}));
        QVERIFY(mainCircle->findIntersectObjs().size() == 2);
        QVERIFY(mainCircle->findIntersectObjs([intersect1](const std::shared_ptr<GObj> & i){
            return i == intersect1;}).size() == 1);
        QVERIFY(mainCircle->findIntersectObjs([intersect2](const std::shared_ptr<GObj> & i){
            return i == intersect2;}).size() == 1);
        QVERIFY(mainCircle->findIntersectObjs([intersectNo](const std::shared_ptr<GObj> & i){
            return i == intersectNo;}).size() == 0);
    }

    void addChild_one_item() {
        auto gObjContainer = std::make_shared<GObj>();
        auto testCircle = gObjContainer->addChild<TestCircle>(std::make_shared<TestCircle>(R, GPos(0,0,0)));
        QVERIFY(testCircle->getR() == R);
        QVERIFY(findByRR(gObjContainer, R)->getR() == R);
    }

    void addChild_multiple_item() {
        auto gObjContainer = std::make_shared<GObj>();
        for (int i = 0; i < N; i++)
            gObjContainer->addChild<TestCircle>(std::make_shared<TestCircle>(i,GPos({(float)i,0,0})));
        for (int i = 0; i < N; i++)
            QVERIFY(findByRR(gObjContainer, i)->getPos().getX() == i);
        QVERIFY(gObjContainer->findChilds(false).size() == (unsigned int)N);
    }

    void remove_removeChild() {
        auto gObjContainer = std::make_shared<GObj>();
        gObjContainer->ADD_CHILD(TestCircle,1, GPos(0,0,0));
        gObjContainer->ADD_CHILD(TestCircle,2, GPos(0,0,0));
        QVERIFY(gObjContainer->findChilds().size() == 2);
        gObjContainer->removeChild(findByRR(gObjContainer, 1));
        QVERIFY_EXCEPTION_THROWN(findByRR(gObjContainer, 1),GObj::cant_find_child);
        QVERIFY(gObjContainer->findChilds().size() == 1);
        findByRR(gObjContainer, 2)->remove();
        QVERIFY(gObjContainer->findChilds().size() == 0);
    }

    void findChildR() {
        auto gObjContainer = std::make_shared<GObj>();

        for (int i = 0; i < N; i++) {
            auto gObjSubContainer = gObjContainer->addChild<GObj>(std::make_shared<GObj>());
            gObjSubContainer->addChild(std::make_shared<TestCircle>(i,GPos({(float)i,0,0})));
        }
        for (int i = 0; i < N; i++)
            QVERIFY(findByRR(gObjContainer, i)->getPos().getX() == i);
    }

    void getRoot() {
        auto gObjContainer = std::make_shared<GObj>();
        auto gObjSubContainer1 = gObjContainer->addChild<GObj>(std::make_shared<GObj>());
        auto gObjSubContainer2 = gObjSubContainer1->addChild<GObj>(std::make_shared<GObj>());
        QVERIFY(gObjSubContainer2->getRoot() == gObjContainer);
    }

    void getChilds_getChildsR() {
        auto gObjContainer = std::make_shared<GObj>();
        for (int i = 0; i < N; i++) {
            auto gObjSubContainer = gObjContainer->addChild<GObj>(std::make_shared<GObj>());
            for (int j = 0; j < N; j++)
                gObjSubContainer->addChild(std::make_shared<TestCircle>(j,GPos({(float)j,0,0})));
        }
        //not recursive
        QVERIFY(gObjContainer->findChilds(false).size() == (unsigned int)N);
        //recursive
        QVERIFY(gObjContainer->findChilds().size() ==(unsigned int)N * (unsigned int)N + (unsigned int)N);
    }

    void cleanupTestCase() {
    }
};

#endif // TEST_OBJ_H
