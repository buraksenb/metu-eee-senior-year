#include  <iostream>
#include "GlobalFunctions.h"
#include <fstream>
#include <string>


// Burak ŞEN 2235646

int main()
{
    std::string line;
    std::string cityList[20];
    std::ifstream myfile ("citylist.txt");
    if (myfile.is_open())
    {
        int i=0;
        while ( std::getline(myfile,line))
        {
            cityList[i]=line;
            i++;
        }
        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file";
	}
	Graph allRoutes;
	FlightTreeNode* allFlights = nullptr;
    // I have added this cities because cities included in route map are not enough for
	// 6-month report with adding 5 routes and flights every month. The reason is that there are no available routes
	// to add and program stucks.
    allRoutes.AddGCity("Analograd");
    allRoutes.AddGCity("Biomedberg");
    allRoutes.AddGCity("Waverium");

	// Below here is the functions which are needed for 6 month report.

	std::cout << "------------Month 1------------\n" << std::endl;
	InitRouteMap(allRoutes);
	InitFlightPlan(allFlights);
	allRoutes.ListGRoutes();

	std::cout << "Flights:\n";
	ListAllFlights(allFlights);
    std::cout << std::endl;
	// String pointer for obtaining reference of first element of new route array
    std::string* spNewRout;
    int N = 5 ;
    int iOccupancyThreshold = 70;
	// Stack for passing routes which will be discounted or discarded to function
    std::stack<std::string> stDiscardedOrDiscountedRoutes;

    std::cout << "------------Month 2------------\n" << std::endl;

    spNewRout = AddMonthlyNewRoutes(allRoutes,cityList,N);
    AddMonthlyNewFlights(allFlights,spNewRout,N);
    std::cout << std::endl;

    stDiscardedOrDiscountedRoutes = CancelUnpopularFlights(allFlights, iOccupancyThreshold);
    std::cout << std::endl;
    DiscardUnpopularRoutesandUpdatePrices(allFlights, allRoutes, cityList, stDiscardedOrDiscountedRoutes);
    std::cout << std::endl;

    allRoutes.ListGRoutes();

	std::cout << "Flights:\n";
	ListAllFlights(allFlights);
    std::cout << "------------Month 3------------\n" << std::endl;
    spNewRout = AddMonthlyNewRoutes(allRoutes,cityList,N);
    AddMonthlyNewFlights(allFlights,spNewRout,N);
    std::cout << std::endl;
    stDiscardedOrDiscountedRoutes = CancelUnpopularFlights(allFlights,iOccupancyThreshold);
    std::cout << std::endl;
    DiscardUnpopularRoutesandUpdatePrices(allFlights,allRoutes,cityList,stDiscardedOrDiscountedRoutes);
    std::cout << std::endl;
    allRoutes.ListGRoutes();

	std::cout << "Flights:\n";
	ListAllFlights(allFlights);
    std::cout << std::endl;

    std::cout << "------------Month 4------------" << std::endl;
    spNewRout = AddMonthlyNewRoutes(allRoutes,cityList,N);
    AddMonthlyNewFlights(allFlights,spNewRout,N);
    std::cout << std::endl;

    stDiscardedOrDiscountedRoutes = CancelUnpopularFlights(allFlights,iOccupancyThreshold);
    std::cout << std::endl;
    DiscardUnpopularRoutesandUpdatePrices(allFlights,allRoutes,cityList,stDiscardedOrDiscountedRoutes);
    std::cout << std::endl;

    allRoutes.ListGRoutes();

	std::cout << "Flights:\n";
	ListAllFlights(allFlights);
    std::cout << std::endl;

    std::cout << "------------Month 5------------\n" << std::endl;
    spNewRout = AddMonthlyNewRoutes(allRoutes,cityList,N);
    AddMonthlyNewFlights(allFlights,spNewRout,N);
    std::cout << std::endl;
    stDiscardedOrDiscountedRoutes = CancelUnpopularFlights(allFlights,iOccupancyThreshold);
    std::cout << std::endl;
    DiscardUnpopularRoutesandUpdatePrices(allFlights,allRoutes,cityList,stDiscardedOrDiscountedRoutes);
    std::cout << std::endl;
    allRoutes.ListGRoutes();

	std::cout << "Flights:\n";
	ListAllFlights(allFlights);
    std::cout << std::endl;
    // String pointer for taking the reference of first element of return routes to be added
    std::string* arrReturnRoutes = nullptr;

    std::cout << "------------Month 6------------" << std::endl;
    spNewRout = AddMonthlyNewRoutes(allRoutes,cityList,N);
    AddMonthlyNewFlights(allFlights,spNewRout,N);
    std::cout << std::endl;
    stDiscardedOrDiscountedRoutes = CancelUnpopularFlights(allFlights,iOccupancyThreshold);
    std::cout << std::endl;
    DiscardUnpopularRoutesandUpdatePrices(allFlights,allRoutes,cityList,stDiscardedOrDiscountedRoutes);
    std::cout << std::endl;

    arrReturnRoutes = CheckandAddReturnRoutes(allRoutes,cityList);
    std::cout << std::endl;
    AddReturnFlights(allFlights,arrReturnRoutes);
    std::cout << std::endl;


    allRoutes.ListGRoutes();
	std::cout << "Flights:\n";
	ListAllFlights(allFlights);

    // Can be used for displaying trip options
    DisplayRouteOptions(allFlights, allRoutes, cityList, "Probia", "Logicheim");
}


