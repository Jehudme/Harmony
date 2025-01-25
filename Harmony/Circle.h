#include "SceneNode.h"
#include "Configuration.h"

namespace harmony
{
    class Circle : public core::SceneNode
    {
    public:
        Circle(const Configuration& configuration);

    private:
        std::shared_ptr<sf::CircleShape> circle;
    };
}
