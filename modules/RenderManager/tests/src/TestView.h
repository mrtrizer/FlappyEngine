#pragma once

//#include <View.h>
//
//class TestView: public flappy::View {
//public:
//    class IMock {
//    public:
//        virtual ~IMock() = default;
//        void virtual draw() = 0;
//        void virtual update() = 0;
//    };
//
//    TestView(IMock* mock = nullptr): m_mock(mock) {}
//    void setMock(IMock* mock) { m_mock = mock; }
//
//protected:
//    void draw(const glm::mat4&, const glm::mat4&) override {
//        if (m_mock)
//            m_mock->draw();
//    }
//    void update(const flappy::PresenterComponent&) override {
//        if (m_mock)
//            m_mock->update();
//    }
//private:
//    IMock* m_mock = nullptr;
//};
