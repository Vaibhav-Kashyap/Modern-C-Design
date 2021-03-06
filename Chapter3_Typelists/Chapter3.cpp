#include "Chapter3.h"

#include "Typelists.h"

#include <iostream>
#include <string>

class IComponentBase
{
public:
	static const std::string name; 
};
const std::string IComponentBase::name = "IComponentBase";

class RenderComponent : public IComponentBase
{
public:
	static const std::string name;
};
const std::string RenderComponent::name = "RenderComponent";

class SphereRenderComponent : public RenderComponent
{
public:
	static const std::string name;
};
const std::string SphereRenderComponent::name = "SphereRenderComponent";

class BoxRenderComponent : public RenderComponent
{
public:
	static const std::string name;
};
const std::string BoxRenderComponent::name = "BoxRenderComponent";

class CubeRenderComponent : public BoxRenderComponent
{
public:
	static const std::string name;
};
const std::string CubeRenderComponent::name = "CubeRenderComponent";

class PhysicsComponent : public IComponentBase
{
public:
	static const std::string name;
};
const std::string PhysicsComponent::name = "PhysicsComponent";

class SpherePhysicsComponent : public PhysicsComponent
{
public:
	static const std::string name;
};
const std::string SpherePhysicsComponent::name = "SpherePhysicsComponent";

template <class T>
class Handle
{
public:
	T value;
};

namespace Working
{
	template<class T>
	class AbstractCollisionEvent
	{
	public:
		virtual void Collision(int id, T& obj) = 0;
		virtual ~AbstractCollisionEvent() {};
	};


	template <class T, class Base>
	class CollisionEventImpl : public Base
	{
		typedef typename Base::ProductList BaseProductList;

	protected:
		typedef typename BaseProductList::Tail ProductList;

	public:
		typedef typename BaseProductList::Head AbstractProduct;
		void Collision(int id, AbstractProduct& obj)
		{
			obj.Collided(id, Chapter2::Type2Type<AbstractProduct>());
		}
	};

	template
	<
		class TList,
		template <class> class Unit = AbstractCollisionEvent
	>
	class CollisionHandlerScatter : public HierarchyGenerators::GenScatterHierarchy<TList, Unit>
	{
	public:
		typedef TList ProductList;

		template<class O>
		void OnCollide(int id, O& obj)
		{
			Unit<O>& unit = *this;
			unit.Collision(id, obj);
		}
		virtual ~CollisionHandlerScatter() {}
	};

	template
	<
		class TList,
		template <class, class> class Handler,
		class AbstractHandler
	>
	class ConcreteCollisionHandler : public  HierarchyGenerators::GenLinearHierarchy<TList, Handler, AbstractHandler>
	{

	};
}

namespace Test
{
	template<class T>
	class AbstractCollision
	{
	public:
		virtual void Collision(int id, T& obj) = 0;
		virtual ~AbstractCollision() {};
	};

	template <class T, class Base>
	class CollisionEventImpl : public Base
	{
	public:
		template<class O>
		void Collision(int id, O& obj)
		{
			obj.Collided(id, Chapter2::Type2Type<O>());
		}
	};

	template
	<
		class TList,
		template <class, class> class Handler,
		class AbstractHandler
	>
	class ConcreteCollisionHandler : public  HierarchyGenerators::GenLinearHierarchy<TList, Handler, AbstractHandler>
	{

	};
}

class Explode
{
public:
	void Collided(int id, Chapter2::Type2Type<Explode>)
	{
		std::cout << "Exploded!";
	}
};
class Spawn
{
public:
	void Collided(int id, Chapter2::Type2Type<Spawn>)
	{
		std::cout << "Spawned!";
	}
};
class Heal
{
public:
	void Collided(int id, Chapter2::Type2Type<Heal>)
	{
		std::cout << "Heal!";
	}
};

void Chapter3::Run()
{
	//RCTL
	typedef TYPELIST_4(BoxRenderComponent, CubeRenderComponent, RenderComponent, SphereRenderComponent)  renderComponentTL;
	//BCTL
	typedef TYPELIST_1(IComponentBase) baseComponentTL;
	//PCTL
	typedef TYPELIST_2(PhysicsComponent, SpherePhysicsComponent) physicsComponentTL;

	std::cout << "Length of Render Component TL: " << TL::Length<renderComponentTL>::value << std::endl;
	std::cout << "Length of Base Component TL: " << TL::Length<baseComponentTL>::value << std::endl;
	std::cout << "Length of Physics Component TL: " << TL::Length<physicsComponentTL>::value << std::endl;
	std::cout << std::endl;

	std::cout << "Type at index 3 in Render Component TL: " << TL::TypeAt<renderComponentTL, 3>::Result::name << std::endl;
	std::cout << "Type at index 0 in Base Component TL: " << TL::TypeAt<baseComponentTL, 0>::Result::name << std::endl;
	std::cout << "Type at index 1 in Physics Component TL: " << TL::TypeAt<physicsComponentTL, 1>::Result::name << std::endl;
	std::cout << std::endl;

	std::cout << "Index of RenderComponent in Render Component TL: " << TL::IndexOf<renderComponentTL, RenderComponent>::value << std::endl;
	std::cout << "Index of PhysicsComponent in Base Component TL: " << (TL::IndexOf<baseComponentTL, PhysicsComponent>::value == -1 ? "NOT FOUND" : std::to_string(TL::IndexOf<baseComponentTL, PhysicsComponent>::value)) << std::endl;
	std::cout << "Index of PhysicsComponent in Physics Component TL: " << TL::IndexOf<physicsComponentTL, PhysicsComponent>::value << std::endl;
	std::cout << std::endl;

	typedef TL::Append<renderComponentTL, physicsComponentTL>::Result derivedTL; //DTL: RCTL + PCTL
	std::cout << "Appened Render Component TL and Physics Component TL, total length : " << TL::Length<derivedTL>::value
		<< ", index of SphereRenderComponent: " << TL::IndexOf<renderComponentTL, SphereRenderComponent>::value << std::endl;
	std::cout << std::endl;

	typedef TL::Erase<derivedTL, SphereRenderComponent>::Result derived2TL; //DTL2: DTL - SRC
	std::cout << "Erasing SphereRenderComponent, new length: " << TL::Length<derived2TL>::value << std::endl;
	std::cout << std::endl;

	typedef TL::Append<derived2TL, renderComponentTL>::Result derived3TL; //DTL3: DTL2 + RCTL
	std::cout << "Appending another Render Component TL, total length: " << TL::Length<derived3TL>::value << std::endl;
	typedef TL::ReplaceAll<derived2TL, BoxRenderComponent, PhysicsComponent>::Result derived4TL; //DTL4: Replace(DTL3, BRC, PC)
	std::cout << "First PhysicsComponent is at index: " << TL::IndexOf<derived3TL, PhysicsComponent>::value << std::endl;
	std::cout << "Replaced all BoxRenderComponents with PhysicsComponent, now first  PhysicsComponent is at index: " << TL::IndexOf<derived4TL, PhysicsComponent>::value << std::endl;
	std::cout << "Index of BoxRenderComponent is: " << (TL::IndexOf<derived4TL, BoxRenderComponent>::value == -1 ? "NOT FOUND" : std::to_string(TL::IndexOf<derived4TL, BoxRenderComponent>::value)) << std::endl;
	std::cout << std::endl;

	typedef TL::EraseAll<derived4TL, PhysicsComponent>::Result derived5TL; //DTL5: EraseAll(DT4, PC)
	std::cout << "Length was: " << TL::Length<derived4TL>::value << "Erased all PhysicsComponents, new length is:  " << TL::Length<derived5TL>::value << std::endl;
	std::cout << "Index of PhysicsComponent is: " << (TL::IndexOf<derived5TL, PhysicsComponent>::value == -1 ? "NOT FOUND" : std::to_string(TL::IndexOf<derived5TL, PhysicsComponent>::value)) << std::endl;
	std::cout << std::endl;

	typedef TL::Append<derived5TL, renderComponentTL>::Result derived6TL; //DTL6: DTL5 + RCTL
	typedef TL::Append<derived6TL, physicsComponentTL>::Result derived7TL; //DTL7: DTL6 + PCTL
	typedef TL::Append<derived7TL, baseComponentTL>::Result fullTL; //FTL: DTL7 + BCTL
	std::cout << "Appened Physics, Render and Base component TLs, length is : " << TL::Length<fullTL>::value << std::endl;
	typedef TL::NoDuplicates<fullTL>::Result cleanTL;//CTL: FTL - Duplicates 
	static const int length = TL::Length<cleanTL>::value;
	std::cout << "Cleaned out duplicates, length: " << TL::Length<cleanTL>::value << std::endl;
	std::cout << std::endl;

	std::cout << "Sorting base to front" << std::endl;
	typedef TL::DerivedToBack<cleanTL>::Result baseAtFrontTL;//BatFTL: DerivedToBack(CTL)
	std::cout << "First type: " << TL::TypeAt<baseAtFrontTL, 0>::Result::name << ", lastType: " << TL::TypeAt<baseAtFrontTL, length - 1>::Result::name << std::endl;

	std::cout << "Sorting base to back" << std::endl;
	typedef TL::DerivedToFront<cleanTL>::Result baseAtBackTL;//BatFTL: DerivedToBack(CTL)
	std::cout << "First type: " << TL::TypeAt<baseAtBackTL, 0>::Result::name << ", last type: " << TL::TypeAt<baseAtBackTL, length - 1>::Result::name << std::endl;
	std::cout << std::endl;

	typedef TYPELIST_4(int, int, std::string, bool) bookTL;
	HierarchyGenerators::GenScatterHierarchy<bookTL, Handle> bookHandler;
	std::cout << "Generating Book class that inherits from int(ID of book), int(edition of book), std::string(name) and bool(currently in inventory)" << std::endl;
	HierarchyGenerators::Field<0>(bookHandler).value = 10;
	HierarchyGenerators::Field<1>(bookHandler).value = 1;
	HierarchyGenerators::Field<2>(bookHandler).value = "Book of Sin";
	HierarchyGenerators::Field<3>(bookHandler).value = false;
	
	std::cout << "Book title: " << HierarchyGenerators::Field<2>(bookHandler).value << ", ID: " << HierarchyGenerators::Field<0>(bookHandler).value <<
		", Edition:  " << HierarchyGenerators::Field<1>(bookHandler).value << ", is currently " << (HierarchyGenerators::Field<3>(bookHandler).value ? "in " : "not in ") << "inventory" << std::endl;
	std::cout << std::endl;

	typedef TYPELIST_3(Spawn, Explode, Heal) colliderTL;
	Spawn s;
	Heal h;
	Explode e;
	//This is based on the Loki implementation of an abstract factory. I don't like it.
	std::cout << "Loki LinearHierarchy" << std::endl;
	typedef Working::CollisionHandlerScatter<colliderTL> WCHS;
	typedef Working::ConcreteCollisionHandler<colliderTL, Working::CollisionEventImpl, WCHS> WCCH;
	WCCH wcch;
	wcch.OnCollide(1, h);
	wcch.OnCollide(1, e);
	wcch.OnCollide(1, s);
	std::cout << std::endl;

	//My attempt at simplifying the Loki implementation
	std::cout << "My LinearHierarchy" << std::endl;
	typedef Test::ConcreteCollisionHandler<colliderTL, Test::CollisionEventImpl, EmptyType> TCCH;
	TCCH tcch;
	tcch.Collision(1, s);
	tcch.Collision(1, e);
	std::cout << std::endl;
}