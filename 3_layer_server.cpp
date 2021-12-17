// #include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
using namespace std;

string const key = "BADC";
map<int,int> keyMap;
/*------------------------------ LAYER 3 STARTS------------------------------------------*/
void setPermutationOrder()
{			
	for(int i=0; i < key.length(); i++)
	{
		keyMap[key[i]] = i;
	}
}
string encrypt3(string msg) {
    
    int row,col,j;
    string cipher = "";
    col = key.length(); 
      
    row = msg.length()/col; 
      
    if (msg.length() % col)
        row += 1;
  
    char matrix[row][col];
  
    for (int i=0,k=0; i < row; i++)
    {
        for (int j=0; j<col; )
        {
            if(msg[k] == '\0')
            {
                matrix[i][j] = '_';     
                j++;
            }
              
            if( isalpha(msg[k]) || msg[k]==' ')
            { 
                matrix[i][j] = msg[k];
                j++;
            }
            k++;
        }
    }
  
    for (map<int,int>::iterator ii = keyMap.begin(); ii!=keyMap.end(); ++ii)
    {
        j=ii->second;
          
        for (int i=0; i<row; i++)
        {
            if( isalpha(matrix[i][j]) || matrix[i][j]==' ' || matrix[i][j]=='_')
                cipher += matrix[i][j];
        }
    }
  
    return cipher;
}
/*------------------------------ LAYER 3 ENDS-------------------------------------------*/
/*-------------------------------LAYER 2 STARTS-----------------------------------------*/
int getNum(vector<int>& v)
{
 
    // Size of the vector
    int n = v.size();
 
    // Generate a random number
    //srand(time(NULL));
 
    // Make sure the number is within
    // the index range
    int index = rand() % n;
 
    // Get random number from the vector
    int num = v[index];
 
    // Remove the number from the vector
    swap(v[index], v[n - 1]);
    v.pop_back();
 
    // Return the removed number
    return num;
}
vector<int> generateRandom(int n)
{
    vector<int> v(n);
    vector<int> r;
    
    // Fill the vector with the values
    // 1, 2, 3, ..., n
    for (int i = 0; i < n; i++)
        v[i] = i + 1;
 
    // While vector has elements
    // get a random number from the vector and print it
    while (v.size()) {
       
        r.push_back(getNum(v));
    }
    return r;
}
string encrypt2(string message , string key) {
    string cipher;
    int d = key.length();
    //cout << "Enter a message to encrypt: ";
    //getline(cin, message);
    //cout << "Enter the key: ";
    //cin >> d;
    
    vector <int> f = generateRandom(d);
    /*cout<<f.size()<<endl;
    for (int i = 0; i < f.size(); i++)
        cout << f[i] << " ";
    
    for (int i = 0; i < d; i++) {
    	cin >> f[i];
    }
    */
    int i = 0; 
    int tr = message.length()%4;
        while(tr--)
        {
            message.push_back(' ');
        }
    
    while (i < message.length()) {
    	for (auto j : f) {
    		if ((j + i - 1) >= message.length() ) {
    			break;
    		}
    		cipher += message[j+i-1];
    	}
    	i += 4;
    }
    return cipher;
}
/*-------------------------------LAYER 2 ENDS-----------------------------------------*/
/*------------------------------ LAYER 1 STARTS------------------------------------------*/
string generateKey(string str, string key)
{
	int x = str.size();

	for (int i = 0; ; i++)
	{
		if (x == i)
			i = 0;
		if (key.size() == str.size())
			break;
		key.push_back(key[i]);
	}
	return key;
}
string encrypt1(string str, string key)
{
	string cipher_text;

	for (int i = 0; i < str.size(); i++)
	{   if(str[i] == ' ')
        {
            char x = ' ';
            cipher_text.push_back(x);
        }
        else{
            // converting in range 0-25
		char x = (str[i] + key[i]+1) %26;//ASSUMING A AS 1
			//REMOVE +1 IF A IS STARTING FROM 0

		// convert into alphabets(ASCII)
		x += 'A';

		cipher_text.push_back(x);

        }
		
	}
	return cipher_text;
}

/*------------------------------ LAYER 1 ENDS------------------------------------------*/
 string encryptHelp()
 {
    string message,key;
    cout << "Enter a message to encrypt: ";
    getline(cin, message);
    cout << "Enter the key: ";
    getline(cin, key);
    string keyadjusted = generateKey(message, key);
    string layer1 = encrypt1(message, keyadjusted);
    cout<<"Layer 1 : "<<layer1<<endl;
    string layer2 = encrypt2(layer1,key);
    cout<<"Layer 2 : "<<layer2<<endl;
    setPermutationOrder();
    string layer3 = encrypt3(layer2);
    cout<<"Layer 3 : "<<layer3<<endl;
    return layer3;
 }

int main() {
    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(7891);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    /*---- Bind the address struct to the socket ----*/
    bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if (listen(welcomeSocket, 5) == 0)
        cout << "Listening" << endl;
    else
        cout << "Error" << endl;
    /*---- Accept call creates a new socket for the incoming connection ----*/
    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

    /*---- Send message to the socket of the incoming connection ----*/
    
    string message = encryptHelp();
    
    char buffer[1024];
    strcpy(buffer, message.c_str());
    printf("Data Encrypted: %s\n", buffer);

    send(newSocket, buffer, 100, 0);
    return 0;
}