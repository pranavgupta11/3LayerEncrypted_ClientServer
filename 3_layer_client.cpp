#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
using namespace std;
string const key = "CADB";
map<int,int> keyMap;
/*-------------------------------LAYER 3--------------------------------------------------*/
void setPermutationOrder()
{			
	// Add the permutation order into map
	for(int i=0; i < key.length(); i++)
	{
		keyMap[key[i]] = i;
	}
}
string decrypt3(string cipher) 
{
	/* calculate row and column for cipher Matrix */
	int col = key.length();

	int row = cipher.length()/col;
	char cipherMat[row][col];

	/* add character into matrix column wise */
	for (int j=0,k=0; j<col; j++)
		for (int i=0; i<row; i++)
			cipherMat[i][j] = cipher[k++];

	/* update the order of key for decryption */
	int index = 0;
	for( map<int,int>::iterator ii=keyMap.begin(); ii!=keyMap.end(); ++ii)
		ii->second = index++;

	/* Arrange the matrix column wise according
	to permutation order by adding into new matrix */
	char decCipher[row][col];
	map<int,int>::iterator ii=keyMap.begin();
	int k = 0;
	for (int l=0,j; key[l]!='\0'; k++)
	{
		j = keyMap[key[l++]];
		for (int i=0; i<row; i++)
		{
			decCipher[i][k]=cipherMat[i][j];
		}
	}

	/* getting Message using matrix */
	string msg = "";
	for (int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			if(decCipher[i][j] != '_')
				msg += decCipher[i][j];
		}
	}
	return msg;
}
/*-------------------------------LAYER 3 ENDS----------------------------------------------*/
/*-------------------------------LAYER 2 STARTS--------------------------------------------*/
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
        //cout << getNum(v) << " ";
        r.push_back(getNum(v));
    }
    return r;
}
string decrypt2(string cipher,string keyHELP) {
	string message;
	int d = keyHELP.length();
    //cout << "Enter the key: ";
    //cin >> d;
    unordered_map <int, int> mapping;
    vector <int> f(d);
    f=generateRandom(d);
    /*for (int i = 0; i < f.size(); i++)
        cout << f[i] << " ";
    
    for (int i = 0; i < d; i++) {
    	cin >> f[i];
    }
    */

    for (int i = 0; i < d; i++) {
    	//cin >> f[i];
    	mapping[f[i]] = i+1;
    }
    vector <int> fInverse(d);
    for (int i = 0; i < d; i++) {
    	fInverse[i] = mapping[mapping[f[i]]];
    }
    int i = 0; 
    while (i < cipher.length()) {
    	for (auto j : fInverse) {
    		if ((j + i - 1) >= cipher.length() ) {
    			break;
    		}
    		message += cipher[j+i-1];
    	}
    	i += 4;
    }
    return message;
}
/*-------------------------------LAYER 2 ENDS-------------------------------------------*/
/*-------------------------------LAYER 1 STARTS--------------------------------------------*/
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
// This function decrypts the encrypted text
// and returns the original text
string decrypt1(string cipher_text, string key)
{
	string orig_text;

	for (int i = 0 ; i < cipher_text.size(); i++)
	{	if(cipher_text[i] == ' ')
        {
            char x = ' ';
            orig_text.push_back(x);
        }
		else{
		// converting in range 0-25
		char x = (cipher_text[i] - key[i] + 26 -1) %26;//ASSUMING A AS 1
			//REMOVE -1 IF A IS STARTING FROM 0
		// convert into alphabets(ASCII)
		x += 'A';
		orig_text.push_back(x);
		}
		
	}
	return orig_text;
}
/*-------------------------------LAYER 1 ENDS-------------------------------------------*/
string decryptHelp(string message)
 {  string keyHELP;
	 cout << "Enter the key: ";
    getline(cin, keyHELP);
	setPermutationOrder();
    string layer3 = decrypt3(message);
	cout<<"Layer 3 : "<<layer3<<endl;
	string layer2 = decrypt2(layer3, keyHELP);
	cout<<"Layer 2 : "<<layer2<<endl;
	string keyadjusted = generateKey(layer2,keyHELP);
	string layer1 = decrypt1(layer2, keyadjusted);
	cout<<"Layer 1 : "<<layer1<<endl;
    return layer1;
 }
int main() {
    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(7891);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
 
    /*---- Read the message from the server into the buffer ----*/
    recv(clientSocket, buffer, 1024, 0);
    /*---- Print the received message ----*/
    cout << "Data recieved: " << buffer << endl;
    string message(buffer);
    
    message = decryptHelp(message);
    cout << "Data Decrypted: " << message <<endl;
    return 0;
}