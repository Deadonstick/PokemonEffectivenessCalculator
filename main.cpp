///Todo: Allow user-input via terminal arguments

#include <iostream>
#include <array>
#include <cstdint>
#include <vector>
#include <cstdio>

using namespace std;

//Types in the game
const unsigned short int amountOfTypes = 18;
//Array to act as link between type and index
const array<const string,amountOfTypes> typeOrder = {"Normal","Fight","Flying","Poison","Ground","Rock","Bug","Ghost","Steel","Fire","Water","Grass","Electric","Psychic","Ice","Dragon","Dark","Fairy"};
//Standard effectiveness modifier table [AttackingType][DefendingType]
const array<const array<float,amountOfTypes> ,amountOfTypes> effectiveness =
{{
    {1,1,1,1,1,0.5,1,0,0.5,1,1,1,1,1,1,1,1,1},
    {2,1,0.5,0.5,1,2,0.5,0,2,1,1,1,1,0.5,2,1,2,0.5},
    {1,2,1,1,1,0.5,2,1,0.5,1,1,2,0.5,1,1,1,1,1},
    {1,1,1,0.5,0.5,0.5,1,0.5,0,1,1,2,1,1,1,1,1,2},
    {1,1,0,2,1,2,0.5,1,2,2,1,0.5,2,1,1,1,1,1},
    {1,0.5,2,1,0.5,1,2,1,0.5,2,1,1,1,1,2,1,1,1},
    {1,0.5,0.5,0.5,1,1,1,0.5,0.5,0.5,1,2,1,2,1,1,2,0.5},
    {0,1,1,1,1,1,1,2,1,1,1,1,1,2,1,1,0.5,1},
    {1,1,1,1,1,2,1,1,0.5,0.5,0.5,1,0.5,1,2,1,1,2},
    {1,1,1,1,1,0.5,2,1,2,0.5,0.5,2,1,1,2,0.5,1,1},
    {1,1,1,1,2,2,1,1,1,2,0.5,0.5,1,1,1,0.5,1,1},
    {1,1,0.5,0.5,2,2,0.5,1,0.5,0.5,2,0.5,1,1,1,0.5,1,1},
    {1,1,2,1,0,1,1,1,1,1,2,0.5,0.5,1,1,0.5,1,1},
    {1,2,1,2,1,1,1,1,0.5,1,1,1,1,0.5,1,1,0,1},
    {1,1,2,1,2,1,1,1,0.5,0.5,0.5,2,1,1,0.5,2,1,1},
    {1,1,1,1,1,1,1,1,0.5,1,1,1,1,1,1,2,1,0},
    {1,0.5,1,1,1,1,1,2,1,1,1,1,1,2,1,1,0.5,0.5},
    {1,2,1,0.5,1,1,1,1,0.5,0.5,1,1,1,1,1,2,2,1}
}};
//Array to contain the effectiveness against every combination of defendingtypes [AttackingType][DefendingPrimaryType][DefendingSecondaryType]
array<array<array<float,amountOfTypes>,amountOfTypes>,amountOfTypes> extendedEffectiveness;
//Vector to contain types the user selected to review
std::vector<uint8_t> selectedTypes;

//Calculates the effectiveness of every defending type combination
void initializeExtendedEffectiveness()
{
    for(int x = 0; x < effectiveness.size(); x++)
    {
        for(int y = 0; y < effectiveness.front().size(); y++)
        {
            for(int z = 0; z < typeOrder.size(); z++)
            {
                if(y!=z)
                    extendedEffectiveness[x][y][z] = effectiveness[x][y] * effectiveness[x][z]; //Every combination type's effectiveness is equal to the product of both types
                else
                    extendedEffectiveness[x][y][z] = effectiveness[x][y]; //Monotype Pokémon have just regular effectiveness
            }
        }
    }
}

void listTypeNumbers() //Lists types with index for the user
{
    for(uint8_t i=0; i< typeOrder.size(); i++)
    {
        cout << +i << ":\t" << typeOrder.at(i) << endl;
    }
}

void typeSelection() //Allows user to select all the Pokémon types present on his team
{
    unsigned short int type = 0;
    while(type<amountOfTypes)
    {
        cout << "Select a type number (or >17 when done): ";
        cin >> type;
        if(type<amountOfTypes)
        {
            selectedTypes.push_back(type);
        }
    }
}

void printSelectedTypes() //Prints the user-selected types
{
    cout << "You have selected the following types: " << endl;
    for(uint8_t i=0; i< selectedTypes.size(); i++)
    {
        cout << typeOrder[selectedTypes.at(i)] << endl;
    }
    cout << endl;
}

//Calculates and prints against which Pokémon monotypes the user-team is super-effective (and which not)
//A user's team is considered to be super-effective against an enemy type if atleast one Pokémon on his team has a super-effective type
void printSuperEffective()
{
    array<bool,amountOfTypes> superEffective = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    cout << "You are super-effective versus: " << endl;
    for(int defType = 0; defType < superEffective.size(); defType++) //For every possible Pokémon type
    {
        for(int i = 0; i < selectedTypes.size(); i++) //For each user-selected type
        {
            if(effectiveness.at(selectedTypes.at(i)).at(defType) > 1.75) //Super-effective is defined as >=2, but to avoid float comparison issues the code compares to 1.75
            {
                superEffective.at(defType) = true;
                cout << typeOrder.at(defType) << endl; //Print which type you are super-effective against
                break;
            }
        }
    }

    cout << endl << "You are NOT super-effective versus: " << endl;
    for(int i = 0; i < superEffective.size(); i++)
    {
        if(!superEffective.at(i))
        {
            cout << typeOrder.at(i) << endl; //Print which type you are not super-effective against
        }
    }
}

//Functions as a 3D-version of printSuperEffective(), prints the team effectiveness against each possible type-combinations of enemy Pokémon
void printExtendedSuperEffective()
{
	//Declare array tracking super-effectiveness of each type-combo
    array<array<bool,amountOfTypes>,amountOfTypes> superEffective;
    for(int i = 0; i < superEffective.size(); i++) //Initialize array to false
    {
        for(int j = 0; j < superEffective.front().size(); j++)
        {
            superEffective[i][j] = false;
        }
    }

    cout << "You are super-effective versus: " << endl;
    for(int defTypeOne = 0; defTypeOne < superEffective.size(); defTypeOne++) //For each possible primary type
    {
        for(int defTypeTwo = 0; defTypeTwo < superEffective.size(); defTypeTwo++) //For each possible secondary type
        {
            for(int i = 0; i < selectedTypes.size(); i++) //For each user-selected type
            {
                if(extendedEffectiveness.at(selectedTypes.at(i)).at(defTypeOne).at(defTypeTwo) > 1.75) //Check if super-effective against typecombo
                {
                    superEffective.at(defTypeOne).at(defTypeTwo) = true;
                    if(defTypeOne!=defTypeTwo)
                    {
                        cout << typeOrder.at(defTypeOne) << "/" << typeOrder.at(defTypeTwo) << endl; //Print type combo
                    }
                    else //If both primary and secondary type are the same the game treats it as monotype
                    {
                        cout << typeOrder.at(defTypeOne) << endl; //Print only monotype
                    }
                    break;
                }
            }
        }
    }

    cout << endl << "You are NOT super-effective versus: " << endl;
    for(int i = 0; i < superEffective.size(); i++)
    {
        for(int j = 0; j < superEffective.size(); j++)
        {
            if(!superEffective.at(i).at(j))
            {
                if(i!=j)
                {
                    cout << typeOrder.at(i) << "/" << typeOrder.at(j) << endl;
                }
                else
                {
                    cout << typeOrder.at(i) << endl;
                }
            }
        }
    }
}

int main()
{
    initializeExtendedEffectiveness();
    listTypeNumbers();
    typeSelection();
    printSelectedTypes();
    printSuperEffective();
    cout << endl <<"And now for the extended types: " << endl;
    printExtendedSuperEffective();
    cin.clear(); //Clear input buffer
    cin.sync(); //Sync input buffer
    cin.ignore(); //Wait for a single input to avoid console closing down immediately (only useful for Windows)
    return 0;
}
