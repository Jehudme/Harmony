#pragma once

#include "Configuration.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "Logger.h"

namespace Harmony {

    class Engine : public Object {
    public:
          Engine();
          Engine(const uint64_t& configuration_id);
          Engine(const std::shared_ptr<Configuration>& configuration);

        void run();

    private:
        void initialize(const Configuration& configuration);

        virtual void event();
        virtual void update();
        virtual void draw();

    protected:

        sf::RenderWindow renderWindow_;
        sf::Clock clock_;
    };

} // namespace Harmony


