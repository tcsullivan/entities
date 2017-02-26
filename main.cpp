#include <iostream>
#include <string>
#include <ctime>
#include <functional>
#include <iomanip>

#include "entities.hpp"

#include <entityx/entityx.h>
#include <entityx/deps/Dependencies.h>

using TestFunc = std::function<void(void)>;
constexpr unsigned int testCount = 100;

void test(const std::string& t, TestFunc ours, TestFunc theirs)
{
	auto run = [](auto f) {
		clock_t avg = 0;
		for (unsigned int i = 0; i < testCount; i++) {
			auto start = clock();
			f();
			auto diff = clock() - start;
			avg += diff;
		}
		auto diff = avg / testCount;
		std::cout << "Test took " <<
			diff << " ticks (" << diff / static_cast<double>(CLOCKS_PER_SEC) <<
			"ms).\n";
		return diff;
	};

	std::cout << std::fixed << std::setprecision(6);
	std::cout << "==============================================\n";
	std::cout << "Running test: " << t << "\n";

	std::cout << "Ours:\n";
	auto o = run(ours);

	std::cout << "Theirs:\n";
	auto p = run(theirs);

	auto percent = (o - p) / static_cast<float>(p);
	std::cout << "\nDIFFERENCE: " << o - p << " ticks (" <<
		std::setprecision(2) << percent << "%).\n\n";
}

struct Position : public Component {
	float x, y;

	Position(float _x = 0, float _y = 0)
		: x(_x), y(_y) {}
};

struct Velocity : public Component {
	float x, y;
	Velocity(float _x = 0, float _y = 0)
		: x(_x), y(_y) {}
};

int main(void)
{
	EntityManager em;

	entityx::EventManager ev;
	entityx::EntityManager xm (ev);
	entityx::SystemManager xsm (xm, ev);

	test("Creation500",
	[&em](){
		for (int i = 0; i < 500; i++)
			em.create();
	},
	[&xm](){
		for (int i = 0; i < 500; i++)
			xm.create();
	});

	test("Reset",
	[&em](){
		em.reset();
	},
	[&xm](){
		xm.reset();
	});

	test("Assign",
	[&em](){
		auto e = em.create();
		auto& pos = *e.assign<Position>(12, 34);

		//std::cout << pos.x << ' ' << pos.y << '\n';
		pos.x = 56, pos.y = 16;
		//std::cout << pos.x << ' ' << pos.y << '\n';
	},
	[&xm](){
		auto e = xm.create();
		auto pos = e.assign<Position>(12, 34);

		//std::cout << pos->x << ' ' << pos->y << '\n';
		pos->x = 56, pos->y = 16;
		//std::cout << pos->x << ' ' << pos->y << '\n';

	});

	test("HasComponent",
	[&em](){
		auto e = em.create();
		//std::cout << e.hasComponent<Position>() << '\n';
		e.assign<Position>();
		//std::cout << e.hasComponent<Position>() << '\n';
	},
	[&xm](){
		auto e = xm.create();
		//std::cout << e.has_component<Position>() << '\n';
		e.assign<Position>();
		//std::cout << e.has_component<Position>() << '\n';
	});

	test("Remove",
	[&em](){
		auto e = em.create();	
		//std::cout << e.hasComponent<Position>() << '\n';
		e.assign<Position>();
		//std::cout << e.hasComponent<Position>() << '\n';
		e.remove<Position>();
		//std::cout << e.hasComponent<Position>() << '\n';
	},
	[&xm](){
		auto e = xm.create();	
		//std::cout << e.has_component<Position>() << '\n';
		e.assign<Position>();
		//std::cout << e.has_component<Position>() << '\n';
		e.remove<Position>();
		//std::cout << e.has_component<Position>() << '\n';
	});

	/*test("EachAll", [&em](){
		em.reset();
		em.create(), em.create(), em.create();
		int i = 0;
		em.each([&i](Entity e) {
			std::cout << ++i << '\n';
		});
	});*/

	test("EachSpec",
	[&em](){
		em.reset();
		em.create(), em.create();
		auto e1 = em.create(); e1.assign<Position>(1, 3);
		em.create();
		auto e2 = em.create(); e2.assign<Position>(99);

		em.each<Position>([](Entity e) {
			auto& pos = *e.component<Position>();
			//std::cout << pos.x << ' ' << pos.y << '\n';
		});
	},
	[&xm](){
		xm.reset();
		xm.create(), xm.create();
		auto e1 = xm.create(); e1.assign<Position>(1, 3);
		xm.create();
		auto e2 = xm.create(); e2.assign<Position>(99);

		xm.each<Position>([](entityx::Entity e, Position& pos) {
			//std::cout << pos.x << ' ' << pos.y << '\n';
		});
	});

	test("Loop100",
	[&em](){
		for (int i = 100; i--;) {
			auto e = em.create();
			e.assign<Position>(i, i);
		}
		em.each<Position>([](Entity e) {
			auto& pos = * e.component<Position>();
			pos.x += 5, pos.y -= 5;
		});
	},
	[&xm](){
		for (int i = 100; i--;) {
			auto e = xm.create();
			e.assign<Position>(i, i);
		}
		xm.each<Position>([](entityx::Entity e, Position pos) {
			pos.x += 5, pos.y -= 5;
		});
	});

	/*test("Dependency",
	[&em](){
		em.addDependency<Velocity, Position>();
		auto e = em.create();
		e.assign<Velocity>();
		std::cout << e.hasComponent<Position>() << e.hasComponent<Velocity>() << '\n';
	},
	[&xm, &xsm](){
		xsm.add<entityx::deps::Dependency<Velocity, Position>>();
		auto e = xm.create();
		e.assign<Velocity>();
		std::cout << e.has_component<Position>() << e.has_component<Velocity>() << '\n';
	});*/

	return 0;
}
