#include "Graph.h"
#include "FlightTreeNode.h"
#include <iostream>
#include <ctime>
#include <stack>
#include <string>
#include <queue>

// Utility function for generating flight codes
std::string flightCodeGenerator(Graph& allRoutes, std::string sDeparted,std::string sArrival)
{
    // Index of the cities are needed for flight code generation
    int iDeparted = allRoutes.SearchGCity(sDeparted);
    int iArrival = allRoutes.SearchGCity(sArrival);

    int iRouteFare = allRoutes.SearchGRoute(sDeparted, sArrival);
    if (iRouteFare == -1)
    {
        // Either one of the cities or route does not exists
        return "[ERROR]: Either one of the cities or route does not exists!!!";
    }
    else
    {
        // If city number is lower than 10 a leading zero must be inserted to flight code
        std::string sDepPr = iDeparted > 10 ? "" : "0";
        std::string sArrPr = iArrival > 10 ? "" : "0";
        std::string flightCode = "EE" + sDepPr + std::to_string(iDeparted) + sArrPr + std::to_string(iArrival);
        return flightCode;
    }
}

void InitRouteMap(Graph& gAllRoutes)
{
    srand((unsigned int)time(0));
    // Add cities with routes on map provided.
    gAllRoutes.AddGCity("Logicheim");
    gAllRoutes.AddGCity("Probia");
    gAllRoutes.AddGCity("Gulf of Archi");
    gAllRoutes.AddGCity("Signale");
    gAllRoutes.AddGCity("Las Datas");

    // since there are initially 6 routes, there will be 6 random numbers
    // for fares of these routes
    int arrRandomNumbers[6];
    for (int i = 0; i < 6; i++)
    {
        arrRandomNumbers[i] = 100 + rand() % 100;
    }
    // Add routes which are on map provided with random fares
    gAllRoutes.AddGRoute("Logicheim", "Probia", arrRandomNumbers[0]);
    gAllRoutes.AddGRoute("Probia", "Logicheim", arrRandomNumbers[1]);
    gAllRoutes.AddGRoute("Signale", "Probia", arrRandomNumbers[2]);
    gAllRoutes.AddGRoute("Gulf of Archi", "Logicheim", arrRandomNumbers[3]);
    gAllRoutes.AddGRoute("Probia", "Las Datas", arrRandomNumbers[4]);
    gAllRoutes.AddGRoute("Probia", "Gulf of Archi", arrRandomNumbers[5]);

}

void ListAllFlights(FlightTreeNode* flightNode)
{
    // Traverse the Binary Tree recursively in left to right order
	if(flightNode != nullptr)
	{
        ListAllFlights(flightNode->getLeftNode());
        std::cout << flightNode->getData() << std::endl;
        ListAllFlights(flightNode->getRightNode());
	}

}


void AddTFlight(FlightTreeNode*& flightNode, std::string flightcode)
{
    FlightTreeNode* newFlight = new FlightTreeNode(flightcode);
    // If Tree is empty enter the first flight as root
    if (flightNode == nullptr)
    {
        flightNode = newFlight;
    }
    else
    {
        FlightTreeNode* tempptr = flightNode;
    	// Iterate through the BinarySearchTree to find place of the newFlight according to the rules
    	// of an binary search tree.
        while (tempptr != nullptr)
        {
        	// When newFlight data is smaller than the node data check if there is a
        	// node at that node's left pointer. If there is no node, than newFlight can be placed there
        	// otherwise continue iterating with that node as basis.
            if (newFlight->getData() < tempptr->getData())
            {

                if (tempptr->getLeftNode() == nullptr)
                {
                    tempptr->setLeftNode(newFlight);
                    break;
                }
                else
                {
                    tempptr = tempptr->getLeftNode();

                }
            }
            // Same as before with the difference being iterate through right
            if (newFlight->getData() > tempptr->getData())
            {
                if (tempptr->getRightNode() == nullptr)
                {
                    tempptr->setRightNode(newFlight);
                    break;
                }
                else
                {
                    tempptr = tempptr->getRightNode();
                }
            }
        }
    }

}

// Checks if flight exists or not and returns true if exists
bool SearchTFlightwithFlightCode(FlightTreeNode* flightNode, std::string flightcode)
{
    FlightTreeNode* temp;
    temp = flightNode;
    while (temp != nullptr)
    {
        if (flightcode == temp->getData())
        {
            return true;
        }
        else if (flightcode < temp->getData())
        {
            temp = temp->getLeftNode();
        }
        else if (flightcode > temp->getData())
        {
            temp = temp->getRightNode();
        }
    }
    return false;
}


// Checks flight exists or not and returns its flight code
std::string SearchTFlight(FlightTreeNode* flightNode, Graph& allRoutes, std::string sDeparted, std::string sArrival)
{
    int iRouteFare = allRoutes.SearchGRoute(sDeparted, sArrival);
    if (iRouteFare == -1)
    {
        // Either one of the cities or route does not exists
        return "[ERROR]: Either one of the cities or route does not exists!!!";
    }
    else
    {
        std::string flightCode = flightCodeGenerator(allRoutes, sDeparted, sArrival);
        bool checkifFlightexits = SearchTFlightwithFlightCode(flightNode, flightCode);

        if (checkifFlightexits)
        {
            return flightCode;
        }
        else
        {
            return "Flight does not exists";
        }

    }

}



// In this part flight node with the corresponding flight code is removed. Since FlightTreeNode
// is heap allocated, it has to be deleted correctly after usage of its done. I could do this with deleting
// two children case. However, when I do it with one children or no children case. Even if I check the pointer is null
// the program tries to access a memory which is deleted before. At this point even if I  assign the pointer to nullptr
// it still tries to access. I receive an error with flightNode->getLeftNode() function with this being 0xDDDDDDDD. Then, program
// crashes with memory access violation. I have commented where I think delete should be.
// Moreover, program does not crash sometimes but it is really a low possibility with deleting nodes.

// Another thing to add that, I really tried to solve this issue. I tried almost 7-8 hours just for this.
FlightTreeNode* RemoveTFlight(FlightTreeNode* flightNode, std::string flightcode)
{

    if (flightNode == nullptr)
    {
        return flightNode;
    }

    if (flightcode < flightNode->getData())
    {
            flightNode->setLeftNode(RemoveTFlight(flightNode->getLeftNode(), flightcode));
            return flightNode;

    }
    else if (flightcode > flightNode->getData())
    {

            flightNode->setRightNode(RemoveTFlight(flightNode->getRightNode(), flightcode));
            return flightNode;


    }
    if (flightNode->getLeftNode() == nullptr) {
        FlightTreeNode* temp = flightNode->getRightNode();
       //delete flightNode;
        flightNode = nullptr;
        return temp;
    }
    else if (flightNode->getRightNode() == nullptr) {

        FlightTreeNode* temp = flightNode->getLeftNode();
         //delete flightNode;
        flightNode = nullptr;
        return temp;

    }

    else {

        FlightTreeNode* parentofChild = flightNode;
        FlightTreeNode* child = flightNode->getRightNode();
        while (child->getLeftNode() != nullptr)
        {
            parentofChild = child;
            child = child->getLeftNode();
        }

        if (parentofChild != flightNode)
        {
            parentofChild->setLeftNode(child->getRightNode());
        }
        else
        {
            parentofChild->setRightNode(child->getRightNode());
        }

        flightNode->setData(child->getData());
        delete child;
        child = nullptr;
        return flightNode;
    }
}

// Initialize flight plan binary search tree with the routes defined in the manual
void InitFlightPlan(FlightTreeNode*& flightNode)
{
    AddTFlight(flightNode, "EE0600");
    AddTFlight(flightNode, "EE0006");
    AddTFlight(flightNode, "EE1400");
    AddTFlight(flightNode, "EE1706");
    AddTFlight(flightNode, "EE0002");
    AddTFlight(flightNode, "EE0017");

}

std::string* AddMonthlyNewRoutes(Graph& allRoutes,std::string cityList[20] ,int N)
{

    std::cout << "Monthly New Route Offers\n";
    std::string* arrnewRoutes = new std::string[N];
    int presentCityIndexs[20];
    int iCountofPresentCities = 0 ;
    int iRandomDeparted;
    int iRandomArrival;
    int iCheckIfRouteExists = -1 ;
    int iRouteFareofNewRoute;
    std::string sRandomDeparted = "";
    std::string sRandomArrival = "";

    for(int i= 0 ; i< 20 ; i++)
    {
    	// Insert all of the indexes of existing cities into an array
        if(allRoutes.checkCityExists(i))
        {
            presentCityIndexs[iCountofPresentCities] = i ;
            iCountofPresentCities++ ;
        }
    }

    while(N != 0)
    {
    iRandomDeparted = rand() % iCountofPresentCities;
    iRandomArrival = rand() % iCountofPresentCities ;
    while(iRandomDeparted == iRandomArrival)
    {
        iRandomArrival = rand() % iCountofPresentCities ;
    }
    // Random values are between 0 to (iCountofPresentCities-1) such that a random departed and arrival city
    // will be obtained from present cities
    sRandomDeparted = cityList[presentCityIndexs[iRandomDeparted]] ;
    sRandomArrival  = cityList[presentCityIndexs[iRandomArrival]];
	// At this point we have to different cities we know they exist
    iCheckIfRouteExists = allRoutes.SearchGRoute(sRandomDeparted,sRandomArrival);
    // If that route does not exists add the route with a random route fare

    if(iCheckIfRouteExists == -1 || iCheckIfRouteExists == 0  )
    {
        iRouteFareofNewRoute = 100+ rand() % 100;
        allRoutes.AddGRoute(sRandomDeparted,sRandomArrival,iRouteFareofNewRoute);
    	// Add flight codes of new routes to an array
        arrnewRoutes[N-1] = flightCodeGenerator(allRoutes,sRandomDeparted,sRandomArrival);
        std::cout << "(" << sRandomDeparted << "---> " << sRandomArrival << ", " << iRouteFareofNewRoute << " )\n";
    	// Decrementing N here, assures that N number of routes will be added definitely.
        N-- ;
    }
    }
	// Return an array of added routes to add flights corresponding to them
    return arrnewRoutes;

}

void AddMonthlyNewFlights(FlightTreeNode*& flightNode,std::string* newRoutes,int N)
{
	// Add flights according to the newRoutes array which holds flight codes of new routes added.
    std::cout << "Monthly New Flight Offers\n" ;
    for(int i= 0 ; i < N ; i ++)
    {
        AddTFlight(flightNode,newRoutes[i]);
        std::cout << "Flight " << newRoutes[i] << " Added\n" ;
    }

}


std::stack<std::string> CancelUnpopularFlights(FlightTreeNode*& flightNode,int OccupancyRateThreshold)
{
    std::stack<std::string> RoutetobeDiscounted;
    std::stack<std::string> RoutetobeRemoved;
    std::stack<FlightTreeNode*> flightNodeStack;
    FlightTreeNode* tempptr = flightNode;
    std::cout << "Cancelled Flights\n" ;
	// First we have to iterate through the flights, then check for occupancy rates
    while (tempptr != nullptr || !flightNodeStack.empty())
    {

        while (tempptr !=  nullptr)
        {

            flightNodeStack.push(tempptr);
             tempptr = tempptr->getLeftNode();
        }

        tempptr = flightNodeStack.top();
        flightNodeStack.pop();

    	// Since every month occupancy rates will change, a random number is generated for occupancy rates to compare
    	// them with the threshold. For %70 threshold, this generated random value has %25 chance to be lower than the threshold
        int newOccupancyRate = 60 + rand() % 40 ;
        tempptr->setOccupancyRate(newOccupancyRate);
        if(tempptr->getOccupancyRate() < OccupancyRateThreshold)
        {
            if(!tempptr->getSuccessiveLowRate())
            {
                // If SuccesiveLowRate is false Route will be discount will be applied
                // And successivelowrate will assigned to True and it is pushed to a stack for not losing the information
                tempptr->setSuccessiveLowRate(true);
                RoutetobeDiscounted.push(tempptr->getData());
            }
            else
            {
                // At this point flight is under the threshold for two months
                // Flight will be removed here and Route will be removed in other function thus it is pushed to a stack
                std::cout << "Flight " << tempptr->getData() << " Cancelled\n" ;
                RoutetobeRemoved.push(tempptr->getData());
                RemoveTFlight(flightNode,tempptr->getData());
            }
        }
        else
        {
            // If occupancy rate is higher than occupancy rate threshold
            // successive rate is assigned to false
            tempptr->setSuccessiveLowRate(false);
        }
    	// Continue iterating
    	if(tempptr != nullptr)
    	{
            tempptr = tempptr->getRightNode();
    	}

    }
    // "DIVIDER" is pushed to stack to separate routes to two which will be discounted and removed
    RoutetobeRemoved.push("DIVIDER");
	// Push the stack which contains routes to be discounted to routes to be removed stack
    while(!RoutetobeDiscounted.empty())
    {
        RoutetobeRemoved.push(RoutetobeDiscounted.top());
        RoutetobeDiscounted.pop();
    }
    return RoutetobeRemoved;
}


void DiscardUnpopularRoutesandUpdatePrices(FlightTreeNode*& allFlights,Graph& allRoutes,std::string cityList[20], std::stack<std::string> unpopularFlights)
{
    std::string flightcode= "";
    int iDeparted = -1 ;
    int iArrival = -1 ;
    int oldRouteFare;
    std::string sDeparted;
    std::string sArrival;
    std::cout << "Discarded Routes & Routes on Discount\n";
    // Discount the routes until DIVIDER term
    while(unpopularFlights.top() != "DIVIDER")
    {

       flightcode = unpopularFlights.top();
       // Obtain indexes and names of the cities from flight codes to discount routes
       iDeparted = (flightcode.at(2)- '0')*10 + (flightcode.at(3)-'0') ;
       iArrival = (flightcode.at(4)- '0')*10 + (flightcode.at(5)-'0') ;
       sDeparted = cityList[iDeparted] ;
       sArrival = cityList[iArrival];
       oldRouteFare = allRoutes.SearchGRoute(sDeparted,sArrival);
       allRoutes.HalveRouteFare(iDeparted,iArrival);
       std::cout << "(" << sDeparted << "---> " << sArrival << ", " << oldRouteFare << " )" << " is 50% Off\n" ;
       unpopularFlights.pop();
    }
    if(unpopularFlights.top() == "DIVIDER")
    {
        unpopularFlights.pop();
    }
    // After the DIVIDER term, remove the remaining routes
    while(!unpopularFlights.empty())
    {
        flightcode = unpopularFlights.top();
		// Obtain indexes and names of the cities from flight codes to remove routes
       iDeparted = (flightcode.at(2)- '0')*10 + (flightcode.at(3)-'0') ;
       iArrival = (flightcode.at(4)- '0')*10 + (flightcode.at(5)-'0') ;
       sDeparted = cityList[iDeparted] ;
       sArrival = cityList[iArrival];
       std::cout << "(" << sDeparted << "---> " << sArrival << ")" << " Route Discarded\n" ;
       allRoutes.RemoveGRoute(sDeparted,sArrival);
       unpopularFlights.pop();
    }
}

std::string* CheckandAddReturnRoutes(Graph& allRoutes,std::string cityList[20])
{
    std::cout << "Missing Return Routes\n";
    std::string* arrReturnRoutes = new std::string[100];
    std::string sDeparted;
    std::string sArrival;
    std::string flightCode;

    int routeCount = 0 ;
    int presentCityIndexs[20];
    int iCountofPresentCities = 0 ;
    int checkIfRouteExists = -1 ;
    int checkIfReturnRouteExists = -1 ;
    int iRouteFareofNewRoute;

    for(int i= 0 ; i< 20 ; i++)
    {
    	// Add existing cities to an array
        if(allRoutes.checkCityExists(i))
        {
            presentCityIndexs[iCountofPresentCities] = i ;
            iCountofPresentCities++ ;
        }
    }
	// Iterate through existing cities
    for(int i=0 ; i<iCountofPresentCities ; i++)
    {
        sDeparted = cityList[presentCityIndexs[i]];
        for(int j = 0 ; j<iCountofPresentCities ; j++)
        {
            sArrival = cityList[presentCityIndexs[j]];
        	// Check if a route exists from a city to another city
            checkIfRouteExists = allRoutes.SearchGRoute(sDeparted,sArrival);
            if(checkIfRouteExists != -1 )
            {
			// Check if there is a return route, if there is not add the return route
            // then add flight code of the route to an array
            checkIfReturnRouteExists = allRoutes.SearchGRoute(sArrival,sDeparted);
            if(checkIfReturnRouteExists == -1)
            {
                iRouteFareofNewRoute =100+ rand() % 100 ;
                allRoutes.AddGRoute(sArrival,sDeparted,iRouteFareofNewRoute );
                flightCode = flightCodeGenerator(allRoutes,sArrival,sDeparted);
                arrReturnRoutes[routeCount] = flightCode;
                routeCount++;
               std::cout << "(" << sDeparted << "---> " << sArrival << ", " << iRouteFareofNewRoute << " ) Return Route Added\n" ;

            }
            }
        }
    }
    // assigning such that end of the flight array can be known
    arrReturnRoutes[routeCount] = "ENDOFARRAY";
    return arrReturnRoutes;
}

void AddReturnFlights(FlightTreeNode*& flightNode, std::string* newReturnRoutes)
{
    std::cout << "Missing Return Flights\n" ;
    int i=0 ;

	// Add new flights using the array obtained from CheckAndAddReturnRoute function
    while(newReturnRoutes[i] != "ENDOFARRAY")
    {
        AddTFlight(flightNode,newReturnRoutes[i]);
        std::cout << "Flight " << newReturnRoutes[i] << " Added\n";
        i++;
    }
    // Since string is heap allocated delete its contents after it is used
    delete[] newReturnRoutes;
}

bool DisplayRouteOptions(FlightTreeNode*& flightNode, Graph& allRoutes,std::string cityList[20], std::string sDeparted, std::string sArrival)
{
    std::cout << "Trip Options for " << sDeparted << " ---> " << sArrival << " : \n" ;
    bool bDirectRoute = false;
    bool bOneStopRoute = false;
    bool bTwoStopRoute = false;
    int iRouteFare1stRoute;
    std::string flightCode1stRoute = "";
    // Check direct trip
    iRouteFare1stRoute = allRoutes.SearchGRoute(sDeparted,sArrival);
    if(iRouteFare1stRoute != -1)
    {
        flightCode1stRoute = flightCodeGenerator(allRoutes,sDeparted,sArrival);
        std::cout << "Direct Trip: " << sDeparted << " ---> " << flightCode1stRoute << "---> "<< sArrival << " for " <<  iRouteFare1stRoute << std::endl;
        bDirectRoute = true;
    }
    std::cout << std::endl;
    // Check indirect trip
    // Since we have adjacency matrix and SearchGRoute function it is not hard to find if there are 1-stop and 2-stop routes
    std::string sActiveCities[20];
    int iCountofCities = 0 ;
    int iRouteFare2ndRoute = 0 ;
    int iRouteFare3rdRoute = 0 ;
    for(int i= 0 ; i < 20 ; i++)
    {
        if(allRoutes.checkCityExists(i))
        {
            if(cityList[i] != sDeparted && cityList[i] != sArrival)
            {
            sActiveCities[iCountofCities] = cityList[i] ;
            iCountofCities++;
            }
        }
    }

    std::queue<std::string> qTwoStopOptions;
    std::string sTwoStopOption;
    std::string flightCode2ndRoute;
    std::string flightCode3rdRoute;
    std::string sPlusop = "+ ";
    std::string sArrow = "---> ";
    std::string sFor= " for ";
    // We have obtained the cities which are in the Routemap except our Departed and Arrival city
    for(int i = 0 ; i< iCountofCities ; i++)
    {
        // Search for any route from departed city
        iRouteFare1stRoute = allRoutes.SearchGRoute(sDeparted,sActiveCities[i]);
        if(iRouteFare1stRoute != -1 && iRouteFare1stRoute != 0)
        {
            flightCode1stRoute = flightCodeGenerator(allRoutes,sDeparted,sActiveCities[i]);
            // If any route is found search for next route which goes to Arrival city for 1-stop
            iRouteFare2ndRoute = allRoutes.SearchGRoute(sActiveCities[i],sArrival);
            if(iRouteFare2ndRoute != -1 && iRouteFare2ndRoute != 0)
            {
            flightCode2ndRoute = flightCodeGenerator(allRoutes,sActiveCities[i],sArrival);
             std::cout << "1-Stop Option " << sDeparted <<  sArrow << flightCode1stRoute <<  sArrow << sActiveCities[i] <<  sArrow<< flightCode2ndRoute
             << sArrow << sArrival << sFor << iRouteFare1stRoute << sPlusop << iRouteFare2ndRoute  <<std::endl;
             bOneStopRoute = true;
            }

            // Then check for 2-stop
            for(int j=0 ; j<iCountofCities ; j++)
            {
            iRouteFare2ndRoute = allRoutes.SearchGRoute(sActiveCities[i],sActiveCities[j]);
            flightCode2ndRoute = flightCodeGenerator(allRoutes,sActiveCities[i],sActiveCities[j]);
            if(iRouteFare2ndRoute != -1 && iRouteFare2ndRoute != 0)
            {
                iRouteFare3rdRoute = allRoutes.SearchGRoute(sActiveCities[j],sArrival);
                flightCode3rdRoute = flightCodeGenerator(allRoutes,sActiveCities[j],sArrival);
                if(iRouteFare3rdRoute != -1 && iRouteFare3rdRoute != 0)
                {
                	// Generate console log message and add it to an queue for printing them after 1-stop options
                    sTwoStopOption = "2-Stop Option " ;
                    sTwoStopOption += sDeparted + sArrow+ flightCode1stRoute+ sArrow+ sActiveCities[i];
                    sTwoStopOption += sArrow + flightCode2ndRoute + sArrow + sActiveCities[j];
                    sTwoStopOption += sArrow+ flightCode3rdRoute  + sArrow + sArrival;
                    sTwoStopOption += sFor + std::to_string(iRouteFare1stRoute) + sPlusop + std::to_string(iRouteFare2ndRoute) +  sPlusop + std::to_string(iRouteFare3rdRoute) ;
                    qTwoStopOptions.push(sTwoStopOption);
                    bTwoStopRoute = true;
                }
            }
            }

        }

    }
    std::cout << std::endl;
	// Print 2-stop options
    while (!qTwoStopOptions.empty())
    {
        std::cout <<  qTwoStopOptions.front() << std::endl;
        qTwoStopOptions.pop();
    }
    if(!(bDirectRoute ||bOneStopRoute ||bTwoStopRoute))
    {
        std::cout << "No Trip Options\n";
        return false;
    }
    return bDirectRoute ||bOneStopRoute ||bTwoStopRoute;
}
