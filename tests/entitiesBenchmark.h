#ifndef ENTITIESBENCHMARK_H_
#define ENTITIESBENCHMARK_H_

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <numeric>
#include <functional>

#include "../entities.hpp"

class EntitiesBenchmark {
    public:

    struct PositionComponent : public Component {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct VelocityComponent : public Component {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct ComflabulationComponent : public Component {
        float thingy = 0.0;
        int dingy = 0;
        bool mingy = false;
        std::string stringy;
    };

    class MovementSystem : public System {
        public:
        MovementSystem() = default;

        void update(EntityManager &es, DeltaTime dt) {
			es.each<PositionComponent, VelocityComponent>(
				[dt](Entity e) {
					auto& pos = *e.component<PositionComponent>();
					auto& vel = *e.component<VelocityComponent>();
					pos.x = vel.x * dt;
					pos.y = vel.y * dt;
				}
			);
        }
    };

    class ComflabSystem : public System {
        public:
        ComflabSystem() = default;

        void update(EntityManager &es, DeltaTime dt) {
   			es.each<ComflabulationComponent>(
				[dt](Entity e) {
					auto comflab = e.component<ComflabulationComponent>();
	                comflab->thingy *= 1.000001f;
	                comflab->mingy = !comflab->mingy;
	                comflab->dingy++;
	                //comflab.stringy = std::to_string(comflab.dingy);
	            }
			);
        }
    };

    #ifdef USE_MORECOMPLEX_SYSTEM
    class MoreComplexSystem : public System {
        private:
        int random(int min, int max){
            // Seed with a real random value, if available
            static std::random_device r;
        
            // Choose a random mean between min and max
            static std::default_random_engine e1(r());

            std::uniform_int_distribution<int> uniform_dist(min, max);

            return uniform_dist(e1);
        }

        public:
        MoreComplexSystem() = default;

        void update(EntityManager &es, DeltaTime dt) {
			es.each<PositionComponent, DirectionComponent, ComflabulationComponent>(
				[dt](Entity e) {
					auto comflab = e.component<ComflabulationComponent>();
					if(comflab) {
						std::vector<double> vec;
						for(size_t i = 0; i < comflab->dingy && i < 100; i++)
							vec.push_back(i * comflab0>thingy);
						int sum = std::accumulate(vec.begin(), vec.end(), 0);
						int product = std::accumulate(vec.begin(), vec.end(),
							1, std::multiplies<double>());
						comflab->stringy = std::to_string(comflab->dingy);
						
						auto pos = e.component<PositionComponent>();
						auto vel = e.component<VelocityComponent>();
						if (pos && vel && comflab->dingy % 10000 == 0) {
							if (pos->x > pos->y) {
								vel->x = random(0, 5);
								vel->y = random(0, 10);
							} else {
								vel->x = random(0, 10);
								vel->y = random(0, 5);
							}
						}
					}
				}
			);
        }
    };
    #endif

    class Application {
        public:
		EntityManager em;
		SystemManager sm;

        Application() : sm(em) {
            sm.add<MovementSystem>();
            sm.add<ComflabSystem>();
            #ifdef USE_MORECOMPLEX_SYSTEM
            sm.add<MoreComplexSystem>();
            #endif
        }

        void update(DeltaTime dt) {
            sm.update<MovementSystem>(dt);
            sm.update<ComflabSystem>(dt);
            #ifdef USE_MORECOMPLEX_SYSTEM
            sm.update<MoreComplexSystem>(dt);
            #endif
        }
    };

    static constexpr double fakeDeltaTime = 1.0 / 60;
};

#endif // ENTITIESBENCHMARK_H_
