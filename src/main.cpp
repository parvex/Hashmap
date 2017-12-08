#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include "LinkedList.h"
#include "HashMap.h"
#include "TreeMap.h"


using namespace aisdi;
void test_inserting();

void test_removing();

void test_dereferencing();

void test_removing();


void perfomTest()
{
	system("clear");
	std::cout << "-----------------------------------------------------------------------\n";
	std::cout << "/Time Compare Test Menu/\n";
	std::cout << "/1/ Compare speed of adding to maps\n";
	std::cout << "/2/ Compare dereference speed\n";
	std::cout << "/3/ Compare remove object speed\n";
	std::cout << "------------------------------------------------------------------------\n";
	std::cout << "Your choice:  ";

	int choice;
	std::cin >> choice;
	if (choice < 1 || choice > 5)
	{
		std::cout << "Select proper operation!";
		return;
	}
	switch (choice)
	{
	case 1:    test_inserting();
		break;
	case 2:    test_dereferencing();
		break;
	case 3:    test_removing();
		break;

	}


}

void test_inserting()
{

	
	aisdi::TreeMap<int, std::string> test_map;
	aisdi::HashMap<int, std::string> test_tree;
	srand(time(NULL));
	auto t1 = std::chrono::high_resolution_clock::now();

	while(test_tree.getSize() < 10000)
		test_tree[rand()] = "test";

	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << "TreeMap inserting took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds\n";

	auto t3 = std::chrono::high_resolution_clock::now();
	while (test_map.getSize() < 10000)
		test_map[rand()] = "test";

	auto t4 = std::chrono::high_resolution_clock::now();

	std::cout << "HashMap inserting took " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " milliseconds\n";
	getchar();
	getchar();

}

void test_dereferencing()
{
	aisdi::TreeMap<int, std::string> test_map;
	aisdi::HashMap<int, std::string> test_tree;
	

	while (test_tree.getSize() < 1000)
		test_tree[rand() % 50000] = "test";
	while (test_map.getSize() < 1000)
		test_map[rand() % 50000] = "test";

	auto t1 = std::chrono::high_resolution_clock::now();
	for(int i = 1000; i < 3000; ++i)
		test_map[i];
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << "| Map dereferencing took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds\n";

	
	auto t3 = std::chrono::high_resolution_clock::now();
	for (int i = 1000; i < 3000; ++i)
		test_tree[i];
	auto t4 = std::chrono::high_resolution_clock::now();

	std::cout << "| Tree dereferencing" << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " milliseconds\n";
	getchar();
	getchar();

}


void test_removing()
{
	aisdi::TreeMap<int, double> test_tree;
	aisdi::HashMap<int, double> test_map;
	while (test_tree.getSize() < 1000)
		test_tree[rand()% 50000] = 2.123123;
	while (test_map.getSize() < 1000)
		test_map[rand()% 50000] = 2.1231312;
	
	for (int i = 0; i < 1000; ++i)
	{
		test_tree[i] = 1.2;
		test_map[i] = 1.2;
	}
	test_tree[30000] = 1;
	test_map[30000] = 1;
	auto t1 = std::chrono::high_resolution_clock::now();
 		test_tree.remove(30000);
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Tree erasing took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds\n";

	auto t3 = std::chrono::high_resolution_clock::now();
		test_map.remove(30000);
	auto t4 = std::chrono::high_resolution_clock::now();
	std::cout << "Map erasing " << std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count() << " milliseconds\n";
	getchar();
	getchar();
}




int main(int argc, char** argv)
{
	const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;

	for (std::size_t i = 0; i < repeatCount; ++i)
		perfomTest();


	return 0;
}

