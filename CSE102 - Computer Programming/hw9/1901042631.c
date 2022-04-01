#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Hospital
{
char* name;
char citiesServed[3];
};

struct card {
	const char *face;
	const char *suit;
};
char *faces[13] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
char *suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};

int n=0;

int numPathsHome(int street, int avenue);
void part1();
int canOfferCovidCoverage(char _cities[6], char _locations[4][3], int _numHospitals, struct Hospital results[4]);
void part2();
void shuffleDeck(struct card deck[52]);
void part3();

void main(void){
	int opt=0;
	while(opt!=4){
		printf("Which part would you like to execute?\n1)Cin Ali's way home\n2)Covid Hospital\n3)Card Shuffle\n4)Quit\n");
		scanf("%d",&opt);
		switch(opt){
			case 1:
				part1();
				break;
			case 2:
				part2();
				break;
			case 3:
				part3();
				break;
			default:
				printf("Please choose a valid option.");
		}
	}
	printf("Goodbye!\n");
}
//--------------------------------------------
void part1(){
	int street,avenue,ans;
	printf("Enter the street number: ");
	scanf("%d",&street);
	printf("Enter the avenue number: ");
	scanf("%d",&avenue);
	ans = numPathsHome(street,avenue);
	printf("Number of optimal paths to take back home: %d\n",ans);
}

int numPathsHome(int street, int avenue){
    if (street == 1 || avenue == 1) return 1; 
    return numPathsHome(street - 1, avenue) + numPathsHome(street, avenue - 1); 
}
//------------------------------------------------
void part2(){
	int hospitalnum;
	struct Hospital results[4];
	char cities[6] = { 'A', 'B', 'C', 'D', 'E', 'F' };
	char locations[4][3] = { {'A', 'B', 'C'}, {'A', 'C', 'D'}, {'B', 'F'}, {'C', 'E', 'F'} };
	printf("Enter the maximum number of hospitals that can be constructed: ");
	scanf("%d",&hospitalnum);


	if(canOfferCovidCoverage(cities,locations,hospitalnum+1,results)==1&& strncmp(results[0].citiesServed,"ABC",3)==0 && strncmp(results[1].citiesServed,"D",1)==0&& strncmp(results[3].citiesServed,"EF",2)==0){
		printf("Yes, can offer health care to all.\n");
		printf("Hospital 1 locations: %.3s\nHospital 2 locations: %.1s\nHospital 3 locations: %.2s\n",results[0].citiesServed,results[1].citiesServed,results[3].citiesServed);
	}
	else
		printf("No, some cities are not covered.");
}

int canOfferCovidCoverage(char _cities[6], char _locations[4][3], int _numHospitals, struct Hospital results[4]){
	if(_numHospitals==0){
		return 1;
	}
	else{
		if(_cities[0]==_locations[0][0]){
			results[0].citiesServed[n]=_cities[0];
			n++;
		}
		else if(_cities[0]==_locations[0][1]){
			results[0].citiesServed[n]=_cities[0];
			n++;
		}
		else if(_cities[0]==_locations[0][2]){
			results[0].citiesServed[n]=_cities[0];
			n++;
		}
		else{
			n=0;
			return canOfferCovidCoverage(_cities,&_locations[1],_numHospitals-1,&results[1]);
		}
		return canOfferCovidCoverage(&_cities[1],_locations,_numHospitals,results);
	}
	return 0;
}

void part3(){
	srand(time(NULL));
	int i=0,j,k=0,shuffler;
	struct card cards[52];
	while(i!=52){
		for(j=0;j<13;j++){             /* INITIALIZING THE DECK IN ORDER*/
			cards[j+i].face = faces[j];
			cards[j+i].suit = suits[k];
		}
		i+=13;
		k++;
	}
	printf("Cards before shuffling:\n");
	for(i=0;i<52;i=i+2){
		printf("%9s of %s\t\t%9s of %s\n",cards[i].face,cards[i].suit,cards[i+1].face,cards[i+1].suit);
	}
	printf("\n---------Type 1 to shuffle-----:\n");

	scanf("%d",&shuffler);
	if(shuffler==1){
		shuffleDeck(cards);
		for(i=0;i<52;i=i+2){
			printf("%9s of %s\t\t%9s of %s\n",cards[i].face,cards[i].suit,cards[i+1].face,cards[i+1].suit);
		}
		printf("^^^^^^^^Succesfully shuffled.\n");
	}
	else printf("You didn'shuffle.");
	printf("\n\n");
}

void shuffleDeck(struct card deck[52]){
  int randCardIndex = 0;
  int i;
  struct card temp = {"", ""};
  for(i=0;i<52;i++){
    randCardIndex = rand() % 52; 
    temp = deck[i];
    deck[i] = deck[randCardIndex];
    deck[randCardIndex] = temp;
  }
}
