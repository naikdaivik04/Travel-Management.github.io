#include <iostream>

#include <fstream>

#include <cstring>

#include <cmath>

#include <ctime>

#include <cstdlib>

#include <iomanip>

#include <conio.h>

#include <algorithm>

#include<vector>

using namespace std;

#define NO_OF_CHARS 256

#define INT_MAX 1000

#define MAX_NODES 51

int adj[1000][1000], acount = 0, ccount = 0, i, j;

int dist[1000][1000], dcount = 0;

int hcount = 0, lcount = 0, rcount = 0;

struct City
{
    int id;
    char name[50];
} S[1000];

struct Hotel
{
    char name[50];
    char place[50];
    float rating;
    int price;
    long long phoneNumber;
    char mail[50];
} ht[100];

struct Lodge
{
    char name[50];
    char location[50];
    int price;
    float rating;
    int stars;
    long long phoneNumber;
} lo[100];

struct Restaurant
{
    char name[50];
    char place[50];
    char type[10];
    float rating;
    char range[50];
    long long phoneNumber;
} re[100];

struct User
{
    char username[50];
    char password[50];
    char fav_location[50];
    char fav_restaurant[50];
    char fav_lodge[50];
};

struct Booking
{
    string name;
    string type;  // hotel, lodge, restaurant
    string placeName;  // Name of the hotel, lodge, or restaurant
    string city;
    string date;
    string time;
    int people;

    void display() const
    {
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Place Name: " << placeName << endl;  // Display the place name
        cout << "City: " << city << endl;
        cout << "Date: " << date << endl;
        cout << "Time: " << time << endl;
        cout << "Number of People: " << people << endl;
    }
};

class BookingSystem
{
private:
    const string fileName = "bookings.txt";  // File to store bookings

    // Helper function to write booking to file
    void writeToFile(const Booking &booking)
    {
        ofstream outFile(fileName, ios::app);  // Append mode

        if (outFile.is_open())
        {
            outFile << booking.name << ","
                    << booking.type << ","
                    << booking.placeName << ","  // Save place name
                    << booking.city << ","
                    << booking.date << ","
                    << booking.time << ","
                    << booking.people << endl;
            outFile.close();
        }
        else
        {
            cout << "Error: Unable to open file to save booking." << endl;
        }
    }

public:
    void addBooking()
    {
        Booking newBooking;

        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, newBooking.name);

        cout << "Enter type (hotel/lodge/restaurant): ";
        getline(cin, newBooking.type);

        // Ask for place name based on the type of booking
        if (newBooking.type == "hotel")
        {
            cout << "Enter the name of the hotel: ";
            getline(cin, newBooking.placeName);
        }
        else if (newBooking.type == "lodge")
        {
            cout << "Enter the name of the lodge: ";
            getline(cin, newBooking.placeName);
        }
        else if (newBooking.type == "restaurant")
        {
            cout << "Enter the name of the restaurant: ";
            getline(cin, newBooking.placeName);
        }
        else
        {
            cout << "Invalid type entered. Booking not added." << endl;
            return;
        }

        cout << "Enter city: ";
        getline(cin, newBooking.city);

        cout << "Enter date (DD/MM/YYYY): ";
        getline(cin, newBooking.date);

        cout << "Enter time (HH:MM): ";
        getline(cin, newBooking.time);

        cout << "Enter number of people: ";
        cin >> newBooking.people;

        writeToFile(newBooking);
        cout << "Booking successfully added and saved to file!" << endl;
    }

    void viewBookingsByCity()
    {
        string cityFilter;

        cout << "Enter city to filter bookings (or type 'ALL' to view all bookings): ";
        cin.ignore();
        getline(cin, cityFilter);

        ifstream inFile(fileName);
        if (inFile.is_open())
        {
            string line;
            bool found = false;

            cout << "\nBookings:" << endl;

            while (getline(inFile, line))
            {
                vector<string> bookingDetails;

                size_t pos = 0;

                // Split line into fields
                while ((pos = line.find(',')) != string::npos)
                {
                    bookingDetails.push_back(line.substr(0, pos));
                    line.erase(0, pos + 1);
                }

                bookingDetails.push_back(line);  // Add the last element

                // Display only bookings matching the city or all if "ALL" is entered
                if (bookingDetails.size() == 7)  // Updated to include placeName
                {
                    Booking booking = {
                        bookingDetails[0],
                        bookingDetails[1],
                        bookingDetails[2],
                        bookingDetails[3],
                        bookingDetails[4],
                        bookingDetails[5],
                        stoi(bookingDetails[6])};

                    if (cityFilter == "ALL" || booking.city == cityFilter)
                    {
                        booking.display();
                        cout << "-----------------------------" << endl;
                        found = true;
                    }
                }
            }

            if (!found)
            {
                cout << "No bookings found for the city: " << cityFilter << endl;
            }
            inFile.close();
        }
        else
        {
            cout << "Error: Unable to open file to read bookings." << endl;
        }
    }
};

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
}

void replaceCharWithSpace(char str[])
{
    int n = strlen(str);

    for (int i = 0; i < n; i++)
    {
        if (str[i] == '-')
        {
            str[i] = ' ';
        }
    }
}

int calorder()
{
    ifstream fp("adjacency_matrix.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file" << endl;
        return 0;
    }

    string line;
    getline(fp, line);

    // Use std::count to count the number of commas
    int row = std::count(line.begin(), line.end(), ',');

    fp.close();
    return row;
}

void load_distance()
{
    ifstream fp("distance_matrix.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file" << endl;
        return;
    }

    int n = calorder();

    int temp;

    char s;

    dcount = n;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fp >> temp >> s;
            dist[i][j] = temp;
        }
    }

    fp.close();
}

void load_adjacency()
{
    ifstream fp("adjacency_matrix.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file" << endl;
        return;
    }

    int n = calorder();

    int temp;

    char s;

    acount = n;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fp >> temp >> s;
            adj[i][j] = temp;
        }
    }

    fp.close();
}

void load_cities()
{
    ifstream city("cities_list.txt");

    if (!city.is_open())
    {
        cout << "\nCannot open the file" << endl;
        return;
    }

    int idd;

    char nam[30];

    while (city >> idd >> nam)
    {
        S[ccount].id = idd;
        strcpy(S[ccount].name, nam);
        ccount++;
    }

    city.close();
}

int minDistance(int dist[], int sptSet[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < dcount; v++)
    {
        if (sptSet[v] == 0 && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void printPath(int parent[], int j)
{
    if (parent[j] == -1)
    {
        return;
    }

    printPath(parent, parent[j]);

    cout << " --> (" << dist[parent[j]][j] << " km) --> " << S[j].name;
}

void printSolution(int dist[], int parent[], int src, int dest)
{
    cout << "\n\nShortest distance from source to destination: " << dist[dest] << " km\n\n";
    cout << "\nAverage Time Taken:\n";
    cout << "Bike : " << dist[dest] / 50 << " Hours\n";
    cout << "Bus : " << dist[dest] / 56 << " Hours\n";
    cout << "Car : " << dist[dest] / 65 << " Hours\n";
    cout << "Train : " << dist[dest] / 60 << " Hours\n";
    cout << "Fastest Train : " << dist[dest] / 95 << " Hours\n";
    cout << "Fastest Bus : " << dist[dest] / 55 << " Hours\n";




    cout << "\nShortest path:\n" << S[src].name;
    printPath(parent, dest);
    cout << "\n\n                                ------------------------------                                      " << endl;
    cout << "                               |   Press enter to continue    |                                     " << endl;
    cout << "                                ------------------------------" << endl;
    cin.ignore();
    cin.get();
    system("cls");
}

void dijkstra(int src, int dest)
{
    int dist[MAX_NODES];

    int sptSet[MAX_NODES];

    int parent[MAX_NODES];

    fill(begin(dist), end(dist), INT_MAX);

    fill(begin(sptSet), end(sptSet), 0);

    fill(begin(parent), end(parent), -1);

    dist[src] = 0;

    for (int count = 0; count < dcount - 1; count++)
    {
        int u = minDistance(dist, sptSet);

        sptSet[u] = 1;

        for (int v = 0; v < dcount; v++)
        {
            if (!sptSet[v] && adj[u][v] && dist[u] != INT_MAX && dist[u] + adj[u][v] < dist[v])
            {
                dist[v] = dist[u] + adj[u][v];
                parent[v] = u;
            }
        }
    }

    printSolution(dist, parent, src, dest);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void badchartable(const char *str, int size, int badchar[NO_OF_CHARS])
{
    fill(badchar, badchar + NO_OF_CHARS, -1);

    for (int i = 0; i < size; i++)
    {
        badchar[(int)str[i]] = i;
    }
}

int checklist(const char* location,char key[])
{
    for (i = 0; i < ccount; i++)
    {
        if (strcasecmp(key, S[i].name) == 0) return S[i].id;
    }

    for (i = 0; i < ccount; i++)
    {
        int m = strlen(key);

        int n = strlen(S[i].name);

        int badchar[NO_OF_CHARS];

        badchartable(key, m, badchar);

        int s = 0;

        while (s <= (n - m))
        {
            int j = m - 1;

            while (j >= 0 && key[j] == S[i].name[s + j])
            {
                j--;
            }

            if (j < 0)
            {
                cout << "\nDid you mean = " << S[i].name;
                return 1000;
            } else {
                s += max(1, j - badchar[S[i].name[s + j]]);
            }
        }
    }

    cout << "\nThe city is not enlisted\n";

    return 1000;
}

void KTM()
{
    string source, desti;

lable:
    cout << "\nEnter the city you are traveling from (Source): ";
    cin >> source;

    if (checklist(source.c_str(), const_cast<char*>(source.c_str())) == 1000) goto lable;

renter:
    cout << "\nEnter the city you are traveling to (Destination): ";
    cin >> desti;

    if (checklist(desti.c_str(), const_cast<char*>(desti.c_str())) == 1000) goto renter;

    dijkstra(checklist(source.c_str(), const_cast<char*>(source.c_str())),
             checklist(desti.c_str(), const_cast<char*>(desti.c_str())));
}

void load_hotels(const string &key)
{
    ifstream fp("hotels.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file";
        return;
    }

    Hotel temp;

    cout << "\n---------------------------------------------------- Hotels in " << key << " ----------------------------------------------------\n\n";
    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
    cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

    while (fp >> temp.name >> temp.place >> temp.rating >> temp.price >> temp.phoneNumber >> temp.mail)
    {
        if (strcasecmp(key.c_str(), temp.place) == 0)
        {
            ht[hcount] = temp;

            replaceCharWithSpace(temp.name);

            replaceCharWithSpace(temp.place);

            cout << "| " << setw(30) << left << temp.name << " | " << setw(20) << left << temp.place << " | "
                 << setw(5) << temp.rating << " | " << setw(5) << temp.price << " | "
                 << setw(15) << temp.phoneNumber << " | " << setw(20) << temp.mail << " |\n";
            cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

            hcount++;
        }
    }
    fp.close();
}

void load_lodges(const string &key)
{
    ifstream fp("lodges.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file";
        return;
    }

    Lodge temp;

    while (fp >> temp.name >> temp.location >> temp.price >> temp.rating >> temp.stars >> temp.phoneNumber)
    {
        if (strcasecmp(key.c_str(), temp.location) == 0)
        {
            if (lcount < 100)
            {
                lo[lcount] = temp;
                lcount++;
            } else {
                cout << "\nMaximum number of lodges reached. Cannot read more data.\n";
                break;
            }
        }
    }

    fp.close();

    cout << "\n---------------------------------------------------- Lodges in " << key << " ----------------------------------------------------\n\n";
    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
    cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

    for (i = 0; i < lcount; i++)
    {
        replaceCharWithSpace(lo[i].name);

        replaceCharWithSpace(lo[i].location);

        cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
             << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
             << setw(15) << lo[i].phoneNumber << " |\n";
        cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
    }
}

void load_restaurants(const string &key)
{
    ifstream fp("restaurants.txt");

    if (!fp.is_open())
    {
        cout << "\nCannot open the file";
        return;
    }

    Restaurant temp;

    rcount = 0;

    while (fp >> temp.name >> temp.place >> temp.type >> temp.rating >> temp.range >> temp.phoneNumber)
    {
        if (strcasecmp(key.c_str(), temp.place) == 0)
        {
            if (rcount < 100)
            {
                re[rcount] = temp;
                rcount++;
            } else {
                cout << "\nMaximum number of restaurants reached. Cannot read more data.\n";
                break;
            }
        }
    }

    fp.close();

    cout << "\n---------------------------------------------------- Restaurants in " << key << " ----------------------------------------------------\n\n";

    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
    cout << "|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n";
    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

    for (i = 0; i < rcount; i++)
    {
        replaceCharWithSpace(re[i].name);

        replaceCharWithSpace(re[i].place);

        cout << "| " << setw(30) << left << re[i].name << " | " << setw(20) << left << re[i].place << " | "
             << setw(6) << re[i].type << " | " << setw(5) << re[i].rating << " | " << setw(6) << re[i].range << " | "
             << setw(15) << re[i].phoneNumber << " |\n";
        cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
    }
}

void mergehotel(Hotel arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;

    int n2 = right - mid;

    Hotel* L = new Hotel[n1];

    Hotel* R = new Hotel[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating >= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;

    delete[] R;
}

void mergesorthotel(Hotel arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesorthotel(arr, left, mid);

        mergesorthotel(arr, mid + 1, right);

        mergehotel(arr, left, mid, right);
    }
}

void filter_hotels()
{
    int choice, min, max, rchoice, pchoice, i;

    while (1)
    {
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "1. Sort based on price\n2. Sort based on rating\n3. Apply a cost range\n4. Back\n";
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            for (i = 1; i < hcount; ++i)
            {
                Hotel key = ht[i];

                int j = i - 1;

                while (j >= 0 && ht[j].price > key.price)
                {
                    ht[j + 1] = ht[j];

                    j = j - 1;
                }
                ht[j + 1] = key;
            }

            cout << "\n1. High to low\n2. Low to high\n   Enter choice: ";
            cin >> pchoice;

            switch (pchoice)
            {
            case 1:
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

                for (i = hcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(ht[i].name);

                    cout << "| " << setw(30) << left << ht[i].name << " | " << setw(20) << left << ht[i].place << " | "
                         << setw(5) << ht[i].rating << " | " << setw(5) << ht[i].price << " | "
                         << setw(15) << ht[i].phoneNumber << " | " << setw(20) << ht[i].mail << " |\n";
                    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                }
                break;

            case 2:
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

                for (i = 0; i < hcount; i++)
                {
                    replaceCharWithSpace(ht[i].name);

                    cout << "| " << setw(30) << left << ht[i].name << " | " << setw(20) << left << ht[i].place << " | "
                         << setw(5) << ht[i].rating << " | " << setw(5) << ht[i].price << " | "
                         << setw(15) << ht[i].phoneNumber << " | " << setw(20) << ht[i].mail << " |\n";
                    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                }
                break;
            }
            break;

        case 2:
            mergesorthotel(ht, 0, hcount - 1);

            cout << "\n1. Best ratings\n2. Least ratings\n   Enter your choice: ";
            cin >> rchoice;

            switch (rchoice)
            {
            case 2:
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

                for (i = hcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(ht[i].name);

                    cout << "| " << setw(30) << left << ht[i].name << " | " << setw(20) << left << ht[i].place << " | "
                         << setw(5) << ht[i].rating << " | " << setw(5) << ht[i].price << " | "
                         << setw(15) << ht[i].phoneNumber << " | " << setw(20) << ht[i].mail << " |\n";
                    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                }
                break;

            case 1:
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
                cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

                for (i = 0; i < hcount; i++)
                {
                    replaceCharWithSpace(ht[i].name);

                    cout << "| " << setw(30) << left << ht[i].name << " | " << setw(20) << left << ht[i].place << " | "
                         << setw(5) << ht[i].rating << " | " << setw(5) << ht[i].price << " | "
                         << setw(15) << ht[i].phoneNumber << " | " << setw(20) << ht[i].mail << " |\n";
                    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

                }
                break;
            }
            break;

        case 3:
            cout << "\nEnter price range(In rupees):\n";
            cout << "Minimum: ";
            cin >> min;

            cout << "Maximum: ";
            cin >> max;

            cout << "\nHotels between price range " << min << " to " << max << " :\n";
            cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
            cout << "|         Name                   |        Place         |Rating | Price |  Phone Number   |         Email            |\n";
            cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";

            for (i = 0; i < hcount; i++)
            {
                if (ht[i].price >= min && ht[i].price <= max)
                {
                    replaceCharWithSpace(ht[i].name);

                    cout << "| " << setw(30) << left << ht[i].name << " | " << setw(20) << left << ht[i].place << " | "
                         << setw(5) << ht[i].rating << " | " << setw(5) << ht[i].price << " | "
                         << setw(15) << ht[i].phoneNumber << " | " << setw(20) << ht[i].mail << " |\n";
                    cout << "+--------------------------------+----------------------+-------+-------+-----------------+--------------------------+\n";
                }
            }
            break;

        default:
            return;
        }
    }
}

void mergelo(Lodge arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;

    int n2 = right - mid;

    Lodge* L = new Lodge[n1];

    Lodge* R = new Lodge[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating >= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;

    delete[] R;
}

void mergesortlo(Lodge arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesortlo(arr, left, mid);

        mergesortlo(arr, mid + 1, right);

        mergelo(arr, left, mid, right);
    }
}

void filter_lodges()
{
    int choice, min, max, rchoice, pchoice, flag, i;

    while (1)
    {
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "1. Sort based on price\n2. Sort in rating\n3. Apply a cost range\n4. Back\n";
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            for (i = 1; i < lcount; ++i)
            {
                Lodge key = lo[i];

                int j = i - 1;

                while (j >= 0 && lo[j].price > key.price)
                {
                    lo[j + 1] = lo[j];
                    j = j - 1;
                }
                lo[j + 1] = key;
            }

            cout << "\n1. High to low\n2. Low to high\n   Enter your choice: ";
            cin >> pchoice;

            switch (pchoice)
            {
            case 1:
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

                for (i = lcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(lo[i].name);

                    cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
                         << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
                         << setw(15) << lo[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                }
                break;

            case 2:
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

                for (i = 0; i < lcount; i++)
                {
                    replaceCharWithSpace(lo[i].name);

                    cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
                         << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
                         << setw(15) << lo[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                }
                break;
            }
            break;

        case 2:
            mergesortlo(lo, 0, lcount - 1);

            cout << "\n1. Best ratings\n2. Least ratings\n   Enter your choice: ";
            cin >> rchoice;

            switch (rchoice)
            {
            case 1:
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

                for (i = 0; i < lcount; i++)
                {
                    replaceCharWithSpace(lo[i].name);

                    cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
                         << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
                         << setw(15) << lo[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                }
                break;

            case 2:
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
                cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

                for (i = lcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(lo[i].name);

                    cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
                         << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
                         << setw(15) << lo[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
                }
                break;
            }
            break;

        case 3:
            cout << "\nEnter price range(In rupees):\n";
            cout << "Minimum: ";
            cin >> min;

            cout << "Maximum: ";
            cin >> max;
            cout << "\n------------Lodges between price range " << min << " to " << max << "------------\n";

            flag = 0;

            cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";
            cout << "|         Name                  |       Location       | Price |Rating | Stars |  Phone Number   |\n";
            cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

            for (i = 0; i < lcount; i++)
            {
                if (lo[i].price >= min && lo[i].price <= max)
                {
                    replaceCharWithSpace(lo[i].name);

                    cout << "| " << setw(30) << left << lo[i].name << " | " << setw(20) << left << lo[i].location << " | "
                         << setw(5) << lo[i].price << " | " << setw(5) << lo[i].rating << " | " << setw(5) << lo[i].stars << " | "
                         << setw(15) << lo[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+-------+-------+-------+-----------------+\n";

                    flag = 1;

                }
            }
            if (flag == 0)
            {
                cout << "\nNo lodges found\n";
            }
            break;

        case 4:
            return;

        default:
            return;
        }
    }
}

void mergert(Restaurant arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;

    int n2 = right - mid;

    Restaurant* L = new Restaurant[n1];

    Restaurant* R = new Restaurant[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].rating <= R[j].rating)
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;

    delete[] R;
}

void mergesortrt(Restaurant arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesortrt(arr, left, mid);

        mergesortrt(arr, mid + 1, right);

        mergert(arr, left, mid, right);
    }
}

void filter_restaurants()
{
    int choice, rchoice, flag, i;

    char filterType[10], filterprice[10];

    while (true)
    {
        cout << "\n\n------------------------\n";
        cout << "1. Sort based on rating\n2. Sort on Veg/NonVeg/Both\n3. Sort based on price\n4. Back\n";
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            mergesortrt(re, 0, rcount - 1);

            cout << "\n1. Best ratings\n2. Least ratings\n   Enter your choice: ";
            cin >> rchoice;

            switch (rchoice)
            {
            case 1:
                cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
                cout << "|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n";
                cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

                for (i = rcount - 1; i >= 0; i--)
                {
                    replaceCharWithSpace(re[i].name);

                    cout << "| " << setw(30) << left << re[i].name << " | " << setw(20) << left << re[i].place << " | "
                         << setw(6) << re[i].type << " | " << setw(5) << re[i].rating << " | " << setw(6) << re[i].range << " | "
                         << setw(15) << re[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
                }
                break;

            case 2:
                cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
                cout << "|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n";
                cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

                for (i = 0; i < rcount; i++)
                {
                    replaceCharWithSpace(re[i].name);

                    cout << "| " << setw(30) << left << re[i].name << " | " << setw(20) << left << re[i].place << " | "
                         << setw(6) << re[i].type << " | " << setw(5) << re[i].rating << " | " << setw(6) << re[i].range << " | "
                         << setw(15) << re[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
                }
                break;
            }
            break;

        case 2:
            cout << "\n   Enter filter type (Veg/Non-Veg/Both): ";
            cin >> filterType;

            cout << "\n----------------filtered list----------------\n";

            flag = 0;

            cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
            cout << "|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n";
            cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

            for (i = 0; i < rcount; i++)
            {
                if (strcasecmp(filterType, re[i].type) == 0)
                {
                    replaceCharWithSpace(re[i].name);

                    cout << "| " << setw(30) << left << re[i].name << " | " << setw(20) << left << re[i].place << " | "
                         << setw(6) << re[i].type << " | " << setw(5) << re[i].rating << " | " << setw(6) << re[i].range << " | "
                         << setw(15) << re[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
                    flag = 1;
                }
            }

            if (flag == 0)
            {
                cout << "\nNo restaurants found\n";
            }
            break;

        case 3:
            cout << "\n   Enter price type (high/average/low): ";
            cin >> filterprice;

            cout << "\n----------------filtered list----------------\n";
            cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";
            cout << "|         Name                  |        Place         |  Type  |Rating | Range  |  Phone Number  |\n";
            cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

            flag = 0;

            for (i = 0; i < rcount; i++)
            {
                if (strcasecmp(filterprice, re[i].range) == 0)
                {
                    replaceCharWithSpace(re[i].name);

                    cout << "| " << setw(30) << left << re[i].name << " | " << setw(20) << left << re[i].place << " | "
                         << setw(6) << re[i].type << " | " << setw(5) << re[i].rating << " | " << setw(6) << re[i].range << " | "
                         << setw(15) << re[i].phoneNumber << " |\n";
                    cout << "+-------------------------------+----------------------+--------+-------+--------+-----------------+\n";

                    flag = 1;

                }
            }

            if (flag == 0)
            {
                cout << "\nNo restaurants found\n";
            }
            break;

        case 4:
            return;

        default:
            cout << "\nInvalid choice. Try again.\n";
        }
    }
}
void info()
{
    int choice, back = 0, temp;
    string city;

    cout << "\n\nEnter the city you want to explore: ";
    cin >> city;

    // Create a modifiable copy of the city string
    char modifiableCity[city.length() + 1];

    strcpy(modifiableCity, city.c_str());

    // Check if the city exists
    if (checklist(modifiableCity, modifiableCity) == 1000)
    {
        cout << "City not found. Please try again.\n";
        return;
    }

opt:
    while (true)
    {
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "\n1. Hotels in " << city;
        cout << "\n2. Lodges in " << city;
        cout << "\n3. Restaurants in " << city;
        cout << "\n4. Go back to home page";
        cout << "\n5. Quit app\n";
        cout << "----------------------------------------------------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            hcount = 0;

            load_hotels(modifiableCity);

            cout << "\n\n1. Go back\n2. Apply filter\n";
            cout << "----------------------------------------------------------------------------------------------\n";
            cout << "Enter your choice: ";
            cin >> temp;

            switch (temp)
            {
            case 1:
                goto opt;
                break;

            case 2:
                filter_hotels();
                break;
            }
            break;

        case 2:
            lcount = 0;

            load_lodges(modifiableCity);

            cout << "\n\n1. Go back\n2. Apply filter\n";
            cout << "----------------------------------------------------------------------------------------------\n";
            cout << "Enter your choice: ";
            cin >> temp;

            switch (temp)
            {
            case 1:
                goto opt;
                break;

            case 2:
                filter_lodges();
                break;
            }
            break;

        case 3:
            rcount = 0;

            load_restaurants(modifiableCity);

            cout << "\n\n1. Go back\n2. Apply filter\n";
            cout << "----------------------------------------------------------------------------------------------\n";
            cout << "Enter your choice: ";
            cin >> temp;

            switch (temp)
            {
            case 1:
                goto opt;
                break;

            case 2:
                filter_restaurants();
                break;
            }
            break;

        case 4:
            back = 1;
            break;

        case 5:
            exit(0);

        default:
            cout << "Invalid input.";
        }

        if (back == 1)
            break;
    }
}


int checkph(long long int *phoneno)
{
    int count = 0;

    long long int temp = (*phoneno);

    while (temp != 0)
    {
        count++;
        temp /= 10;
    }

    return count;
}

void enlisth()
{
    ofstream file("hotels.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nError opening the file.\n";
        return;
    }

    string name, place, email;

    float rating;

    int price;

    long long phoneNumber;

    cout << "\n--------------------------------";
    cout << "\nEnter Details of Hotel to Enlist\n";
    cout << "Name: ";
    cin >> ws; // To clear leading whitespace
    getline(cin, name);

    cout << "Location: ";
    cin >> ws;
    getline(cin, place);

    cout << "Rating: ";
    cin >> rating;

    cout << "Price: ";
    cin >> price;

    while (true)
    {
        cout << "Phone number: ";
        cin >> phoneNumber;

        if (checkph(&phoneNumber) == 10)
        {
            break;
        }
        cout << "Not a valid phone number. Please try again.\n";
    }

    while (true)
    {
        cout << "Email: ";
        cin >> email;

        if (email.rfind("@gmail.com") != string::npos)
        {
            break;
        }
        cout << "Not a valid Gmail ID. Please try again.\n";
    }

    file << name << " " << place << " " << fixed << setprecision(1) << rating << " " << price << " "
         << phoneNumber << " " << email << endl;

    cout << "\n\nHotel information added successfully!\n";

    delay(3);

    system("cls");
}

void enlistl()
{
    ofstream file("lodges.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nError opening the file.\n";
        return;
    }

    string name, location;

    float rating;

    int price, stars;

    long long phoneNumber;

    cout << "\n--------------------------------";
    cout << "\nEnter Details of Lodge to Enlist\n";
    cout << "Name: ";
    cin >> ws;
    getline(cin, name);

    cout << "Location: ";
    cin >> ws;
    getline(cin, location);

    cout << "Price: ";
    cin >> price;

    cout << "Rating: ";
    cin >> rating;

    cout << "Stars: ";
    cin >> stars;

    while (true)
    {
        cout << "Phone number: ";
        cin >> phoneNumber;

        if (checkph(&phoneNumber) == 10)
        {
            break;
        }
        cout << "Not a valid phone number. Please try again.\n";
    }

    file << name << " " << location << " " << price << " " << fixed << setprecision(1) << rating << " "
         << stars << " " << phoneNumber << endl;

    cout << "\n\nLodge information added successfully!\n";

    delay(3);

    system("cls");
}

void enlistr()
{
    ofstream file("restaurants.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nError opening the file.\n";
        return;
    }

    string name, place, type, range;

    float rating;

    long long phoneNumber;

    cout << "\n-------------------------------------";
    cout << "\nEnter Details of Restaurant to Enlist\n";
    cout << "Name: ";
    cin >> ws;
    getline(cin, name);

    cout << "Location: ";
    cin >> ws;
    getline(cin, place);

    cout << "Type: ";
    cin >> ws;
    getline(cin, type);

    cout << "Rating: ";
    cin >> rating;

    cout << "Price range: ";
    cin >> ws;
    getline(cin, range);

    while (true)
    {
        cout << "Phone number: ";
        cin >> phoneNumber;
        if (checkph(&phoneNumber) == 10)
        {
            break;
        }
        cout << "Not a valid phone number. Please try again.\n";
    }

    file << name << " " << place << " " << type << " " << fixed << setprecision(1) << rating << " "
         << range << " " << phoneNumber << endl;

    cout << "\n\nRestaurant information added successfully!\n";

    delay(3);

    system("cls");
}

int loginUser(const string& username, const string& password)
{
    ifstream file("user_credentials.txt");

    if (!file.is_open())
    {
        cout << "\nError opening user credentials file.\n";
        return 0;
    }

    User user;

    while (file >> user.username >> user.password)
    {
        if (username == user.username && password == user.password)
        {
            file.close();
            return 1;
        }
    }

    file.close();

    return 0;
}

// Function to create a new user
void createUser()
{
    ifstream infile("user_credentials.txt");

    if (!infile.is_open())
    {
        // If the file doesn't exist, create it
        ofstream outfile("user_credentials.txt");

        if (!outfile.is_open())
        {
            cout << "\nError creating the file.\n";
            return;
        }
        outfile.close();
    }

    ofstream outfile("user_credentials.txt", ios::app);

    if (!outfile.is_open())
    {
        cout << "\nError opening file.\n";
        return;
    }

    string uname, pass, secAnswer, existingUname, existingPass, existingSecAnswer;

tryagain:
    cout << "\n                                          New Account\n";
    cout << "                                      Username: ";
    cin >> uname;

    infile.clear();

    infile.seekg(0, ios::beg); // Reset file to the beginning

    // Check if username already exists
    while (infile >> existingUname >> existingPass >> existingSecAnswer)
    {
        if (uname == existingUname)
            {
            cout << "\nUsername already exists, try a different username.\n";
            goto tryagain;
        }
    }

    cout << "\n                                      Password: ";
    cin >> pass;
    cout << "\n                              Set a Security Question Answer:";
    cout << "\n                         (Example: What is your favorite color?) :- \n ";
    cin.ignore();
    getline(cin, secAnswer);

    // Save the new user's details
    outfile << uname << " " << pass << " " << secAnswer << endl;

    cout << "\n                              User account created successfully!\n";

    infile.close();

    outfile.close();
}

// Function to load user preferences
void loadUserPreferences(const string& username)
{
    ifstream file("user_preferences.txt");

    if (!file.is_open())
    {
        cout << "\nError opening file.\n";
        return;
    }

    User user;

    bool flag = false;

    while (file >> user.username >> user.fav_location >> user.fav_restaurant >> user.fav_lodge)
    {
        if (username == user.username)
        {
            flag = true;
            cout << "\nFavourite location:   " << user.fav_location << "\n";
            cout << "Favourite restaurant: " << user.fav_restaurant << "\n";
            cout << "Favourite Lodge:      " << user.fav_lodge << "\n";
        }
    }

    if (!flag)
    {
        cout << "\nNo favourites\n";
    }

    cout << "\n\n                                ------------------------------\n";
    cout << "                               |   Press enter to continue    |\n";
    cout << "                                ------------------------------";
    cin.ignore();
    cin.get(); // Pause for user input
    file.close();
}

// Function to save user preferences
void saveUserPreferences(const string& username)
{
    ofstream file("user_preferences.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nError opening file.\n";
        return;
    }

    string loc, rest, lod;

    cout << "\nEnter your Favourite\nLocation: ";
    cin >> ws;
    getline(cin, loc);

    cout << "Restaurant: ";
    getline(cin, rest);

    cout << "Lodge: ";
    getline(cin, lod);

    file << username << " " << loc << " " << rest << " " << lod << endl;

    cout << "\nUser preferences saved successfully!\n";
    delay(3); // Assuming delay is implemented elsewhere
    file.close();
}

void addfeedback(const string& username)
{
    int choice;

    string name, feedbackDesc;

    ofstream file("reviews.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nError opening file.\n";
        return;
    }

    while (true)
    {
        cout << "\nFeedback on:\n1. Hotel\n2. Lodge\n3. Restaurant\nEnter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\nEnter Hotel name: ";
            cin >> ws;
            getline(cin, name);
            cout << "\nWrite your feedback: ";
            getline(cin, feedbackDesc);
            file << 'h' << " " << name << " " << feedbackDesc << endl;
            break;

        case 2:
            cout << "\nEnter Lodge name: ";
            cin >> ws;
            getline(cin, name);
            cout << "\nWrite your feedback: ";
            getline(cin, feedbackDesc);
            file << 'l' << " " << name << " " << feedbackDesc << endl;
            break;

        case 3:
            cout << "\nEnter Restaurant name: ";
            cin >> ws;
            getline(cin, name);
            cout << "\nWrite your feedback: ";
            getline(cin, feedbackDesc);
            file << 'r' << " " << name << " " << feedbackDesc << endl;
            break;

        default:
            cout << "\nInvalid choice. Please try again.\n";
            continue;
        }

        cout << "\n\nFeedback uploaded.\n";
        delay(3); // Assuming delay is implemented elsewhere
        file.close();
        return;
    }
}

// Function to load feedbacks
void loadfeedbacks(const string& username)
{
    int choice;

    string code, name, feedbackDesc;

    bool flag = false;

    ifstream file("reviews.txt");

    if (!file.is_open())
    {
        cout << "\nCannot access the database.\n";
        return;
    }

    while (true)
    {
        cout << "\nSee all Feedbacks\n";
        cout << "1. Hotel\n2. Lodge\n3. Restaurant\n4. Back\nEnter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            flag = false;
            file.clear();
            file.seekg(0, ios::beg); // Reset to the beginning of the file

            while (file >> code >> ws && getline(file, name, ' ') && getline(file, feedbackDesc))
            {
                if (code == "h")
                {
                    cout << "\nHotel Name: " << name << "\nFeedback: " << feedbackDesc << "\n";
                    flag = true;
                }
            }

            if (!flag)
            {
                cout << "\nNo feedbacks on hotels.\n";
            }
            break;

        case 2:
            flag = false;
            file.clear();
            file.seekg(0, ios::beg);

            while (file >> code >> ws && getline(file, name, ' ') && getline(file, feedbackDesc))
            {
                if (code == "l")
                {
                    cout << "\nLodge Name: " << name << "\nFeedback: " << feedbackDesc << "\n";
                    flag = true;
                }
            }

            if (!flag)
            {
                cout << "\nNo feedbacks on lodges.\n";
            }
            break;

        case 3:
            flag = false;
            file.clear();
            file.seekg(0, ios::beg);

            while (file >> code >> ws && getline(file, name, ' ') && getline(file, feedbackDesc))
            {
                if (code == "r")
                {
                    cout << "\nRestaurant Name: " << name << "\nFeedback: " << feedbackDesc << "\n";
                    flag = true;
                }
            }

            if (!flag)
            {
                cout << "\nNo feedbacks on restaurants.\n";
            }
            break;

        case 4:
            file.close();
            return;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }
}

void getPassword(std::string &password, int max_length)
{
    int i = 0;

    char ch;

    password.clear(); // Clear any existing content

    while (true)
    {
        ch = getch();

        if (ch == 13 || ch == 10)
        { // Enter key
            break;
        } else if (ch == 8 && !password.empty())
        { // Backspace
            i--;
            password.pop_back();
            std::cout << "\b \b";
        } else
        if (i < max_length - 1)
        {
            password.push_back(ch);
            i++;
            std::cout << "*";
        }
    }
    std::cout << std::endl;
}

void resetUserPassword(const string &username, const string &newPassword);

void resetUserPassword(const string &username, const string &newPassword)
{
    ifstream infile("user_credentials.txt");

    ofstream tempFile("temp.txt");

    if (!infile.is_open() || !tempFile.is_open())
    {
        cout << "\nError opening file for password reset.\n";
        return;
    }

    string existingUname, existingPass, existingSecAnswer;

    bool userFound = false;

    // Read the original file and write to the temporary file
    while (infile >> existingUname >> existingPass >> existingSecAnswer)
    {
        if (username == existingUname)
        {
            userFound = true;
            tempFile << existingUname << " " << newPassword << " " << existingSecAnswer << "\n"; // Update password
        } else {
            tempFile << existingUname << " " << existingPass << " " << existingSecAnswer << "\n"; // Copy as is
        }
    }

    infile.close();
    tempFile.close();

    if (userFound)
    {
        // Replace the original file with the updated one
        remove("user_credentials.txt");
        rename("temp.txt", "user_credentials.txt");
    } else {
        remove("temp.txt"); // Remove temporary file if user not found
        cout << "\nUsername not found. Password reset failed.\n";
    }
}

void forgotPassword()
{
    ifstream infile("user_credentials.txt");
    if (!infile.is_open())
    {
        cout << "\nError opening file. Please ensure the user credentials file exists.\n";
        return;
    }

    string uname, secAnswer, newPass;

    string existingUname, existingPass, existingSecAnswer;

    bool found = false;

    cout << "\n                                   Forgot Password\n";
    cout << "                                      Username: ";
    cin >> uname;

    // Search for the username in the file
    while (infile >> existingUname >> existingPass >> existingSecAnswer)
    {
        if (uname == existingUname)
        {
            found = true;
            cout << "\nAnswer your security question: What is your favorite color? ";
            cin.ignore();
            getline(cin, secAnswer);

            if (secAnswer == existingSecAnswer)
            {
                cout << "\nEnter your new password: ";
                cin >> newPass;

                infile.close(); // Close the file before rewriting
                resetUserPassword(uname, newPass);
                cout << "\nPassword reset successfully!\n";
                return;
            } else {
                cout << "\nIncorrect security answer. Password reset failed.\n";
                return;
            }
        }
    }

    infile.close();

    if (!found)
    {
        cout << "\nUsername not found. Please check and try again.\n";
    }
}

void logLogin(const string &username)
{
    ofstream logFile("login_log.txt", ios::app); // Open in append mode

    if (!logFile.is_open())
    {
        cout << "\nError opening log file.\n";
        return;
    }

    // Get the current time
    time_t now = time(0);

    char *dt = ctime(&now);

    // Write the username and timestamp to the log file
    logFile << "Username: " << username << ", Login Time: " << dt;

    logFile.close();

    cout << "\nLogin activity recorded successfully.\n";
}

int main()
{
    BookingSystem bookingSystem; // Create a booking system instance

    int choice, adminChoice, userChoice, feedbackChoice, bookingChoice, maxLength = 20;

    std::string username, password;

    load_adjacency();

    load_distance();

    load_cities();

    while (true)
    {
        std::cout << "\n ===============================================================================================\n"
                  << "|                                                                                              |\n"
                  << "|                              Welcome to Travel Management System                             |\n"
                  << "|                                                                                              |\n"
                  << " ===============================================================================================\n"
                  << "\n1. Login"
                  << "\n2. Create an Account"
                  << "\n3. Forgot Password"
                  << "\n4. Quit\n"
                  << "----------------------------------------------------------------------------------------------\n"
                  << "Enter your choice: ";
        std::cin >> adminChoice;

        switch (adminChoice)
        {
        case 1:
            std::cout << "\n ===============================================================================================\n"
                      << "|                                                                                              |\n"
                      << "|                                            Login                                             |\n"
                      << "|                                                                                              |\n"
                      << " ===============================================================================================\n";
            std::cout << "\nUsername: ";
            std::cin >> username;
            std::cout << "\nPassword: ";
            getPassword(password, maxLength);
            system("cls");

            if (loginUser(username, password))
            {
                logLogin(username);
                std::cout << "\nWelcome, " << username << "!\n";
                delay(2);
                system("cls");
            } else {
                std::cout << "\nInvalid username or password. Please try again.\n";
                delay(2);
                system("cls");
                continue; // Retry login/signup
            }
            break;

        case 2:
            std::cout << "\n ===============================================================================================\n"
                      << "|                                                                                              |\n"
                      << "|                                       Create Account                                         |\n"
                      << "|                                                                                              |\n"
                      << " ===============================================================================================\n";
            createUser();
            delay(2);
            system("cls");
            continue; // Retry login/signup

        case 3:
             std::cout << "\n ===============================================================================================\n"
                      << "|                                                                                              |\n"
                      << "|                                      Forgot Password                                         |\n"
                      << "|                                                                                              |\n"
                      << " ===============================================================================================\n";
            std::cout << "\nTo reset your password, you'll need to answer your security question.\n";
            forgotPassword();
            delay(2);
            system("cls");
            break;

        case 4:
            std::cout << "\nThank you for using our system. Goodbye!\n";
             delay(2);
       #ifdef _WIN32
               system("exit"); // Close the terminal on Windows
      #else
          exit(0); // Exit for other platforms
     #endif
         break;
            //return 0;

        default:
            std::cout << "\nInvalid choice. Please select from the available options.\n";
            continue; // Retry login/signup
        }

        // Logged-in Menu
        while (true)
        {
            std::cout << "\n ===============================================================================================\n"
                      << "|                                                                                              |\n"
                      << "|                             Main Menu - Explore Your Options                                 |\n"
                      << "|                                                                                              |\n"
                      << " ===============================================================================================\n"
                      << "\n1. Plan a Trip (Travel Routes)"
                      << "\n2. Search and Explore Place"
                      << "\n3. Enlist a New Hotel, Lodge, or Restaurant"
                      << "\n4. Booking Menu" // New option added
                      << "\n5. User Dashboard (Favorites & Feedback)"
                      << "\n6. Log Out\n"
                      << "----------------------------------------------------------------------------------------------\n"
                      << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                KTM();
                break;

            case 2:
                info();
                break;

            case 3:
                system("cls");
                std::cout << "\n ===============================================================================================\n"
                          << "|                                                                                              |\n"
                          << "|                                     Enlist Your Business                                     |\n"
                          << "|                                                                                              |\n"
                          << " ===============================================================================================\n"
                          << "\n1. Enlist a Hotel"
                          << "\n2. Enlist a Lodge"
                          << "\n3. Enlist a Restaurant\n"
                          << "-----------------------------------------------------------------------------------------------\n"
                          << "Enter your choice: ";
                std::cin >> choice;

                if (choice == 1) enlisth();
                else if (choice == 2) enlistl();
                else if (choice == 3) enlistr();
                else std::cout << "Invalid choice. Returning to main menu.\n";
                break;

            case 4: // Booking Menu
                while (true)
                {
                    std::cout << "\n ===============================================================================================\n"
                              << "|                                                                                              |\n"
                              << "|                                   Booking Menu                                               |\n"
                              << "|                                                                                              |\n"
                              << " ===============================================================================================\n"
                              << "\n1. Add a Booking"
                              << "\n2. View Bookings by City"
                              << "\n3. Back to Main Menu\n"
                              << "----------------------------------------------------------------------------------------------\n"
                              << "Enter your choice: ";

                    std::cin >> bookingChoice;

                    switch (bookingChoice)
                    {
                    case 1:
                        bookingSystem.addBooking();
                        break;
                    case 2:
                        bookingSystem.viewBookingsByCity();
                        break;
                    case 3:
                        goto mainMenu; // Return to main menu
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                    }
                }
            mainMenu:
                break;

            case 5:
                 while (true)
                {
                    std::cout << "\n ===============================================================================================\n"
                              << "|                                                                                              |\n"
                              << "|                                   User Dashboard                                             |\n"
                              << "|                                                                                              |\n"
                              << " ===============================================================================================\n"
                              << "\n1. Add Feedback"
                              << "\n2. View Feedbacks"
                              << "\n3. Back to Main Menu\n"
                              << "----------------------------------------------------------------------------------------------\n"
                              << "Enter your choice: ";
                    std::cin >> feedbackChoice;

                    switch (feedbackChoice)
                    {
                    case 1:
                        addfeedback(username);
                        break;
                    case 2:
                        loadfeedbacks(username);
                        break;
                    case 3:
                        goto userDashboard; // Back to main menu
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                    }
                }
            userDashboard:
                break;

            case 6:
                char confirm;

                std::cout << "\nAre you sure you want to log out? (Y/N): ";
                std::cin >> confirm;

                if (toupper(confirm) == 'Y')
                {
                    std::cout << "\nLogging out. See you soon, " << username << "!\n";
                    delay(2);
                    #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
                    exit(0); // End the program
            }
            else
            {
                std::cout << "\nReturning to the main menu.\n";
            }
            break;
        }
    }
  }
}














