#include <iostream>
#include <fstream>
#include <string>
#include <cctype> 

using namespace std;

void writeFile(const string& fileName) {
    cout << "Do you want to (a)ppend or (o)verwrite the file? (Enter 'a' or 'o'): ";
    char choice;
    cin >> choice;
    cin.ignore();  

    ios::openmode mode = (choice == 'a' || choice == 'A') ? ios::app : ios::trunc;
    ofstream outFile(fileName.c_str(), mode);  

    if (!outFile) {
        cerr << "Error: Unable to open file for writing: " << fileName << endl;
        return;
    }

    cout << "Enter the text you want to write to the file (end input with an empty line):" << endl;

    string line;
    while (true) {
        getline(cin, line);
        if (line.empty()) break;  
        outFile << line << endl;  
    }

    outFile.close();
    cout << "File written successfully: " << fileName << endl;
}

void compressFile(const string& inputFileName, const string& outputFileName) {
    ifstream inFile(inputFileName.c_str(), ios::binary);  
    ofstream outFile(outputFileName.c_str(), ios::binary);  

    if (!inFile) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return;
    }
    if (!outFile) {
        cerr << "Error: Unable to open output file: " << outputFileName << endl;
        return;
    }

    char currentChar;
    char previousChar = '\0';
    int count = 0;

    if (inFile.peek() == ifstream::traits_type::eof()) {
        cerr << "Warning: Input file is empty. Nothing to compress." << endl;
        inFile.close();
        outFile.close();
        return;
    }

    cout << "Compressing file..." << endl;

    while (inFile.get(currentChar)) {
        if (isspace(currentChar)) {
            continue;  
        }

        if (currentChar == previousChar) {
            count++;
        } else {
            if (count > 0) {
                outFile << previousChar << count;  
                cout << "Writing: " << previousChar << " with count " << count << endl;
            }
            previousChar = currentChar;
            count = 1;
        }
    }

    if (count > 0) {
        outFile << previousChar << count;
        cout << "Writing: " << previousChar << " with count " << count << endl;
    }

    inFile.close();
    outFile.close();

    cout << "Compression completed." << endl;
}

void decompressFile(const string& inputFileName, const string& outputFileName) {
    ifstream inFile(inputFileName.c_str(), ios::binary);  
    ofstream outFile(outputFileName.c_str(), ios::binary);  

    if (!inFile) {
        cerr << "Error: Unable to open input file: " << inputFileName << endl;
        return;
    }
    if (!outFile) {
        cerr << "Error: Unable to open output file: " << outputFileName << endl;
        return;
    }

    char currentChar;
    int count;

    if (inFile.peek() == ifstream::traits_type::eof()) {
        cerr << "Warning: Input file is empty. Nothing to decompress." << endl;
        inFile.close();
        outFile.close();
        return;
    }

    cout << "Decompressing file..." << endl;

    while (inFile.get(currentChar)) {
        if (isdigit(currentChar)) {
            cerr << "Error: Invalid format in compressed file. Unexpected digit found." << endl;
            inFile.close();
            outFile.close();
            return;
        }
        if (inFile >> count && count > 0) {  
            for (int i = 0; i < count; i++) {
                outFile.put(currentChar);
            }
            cout << "Decompressed: " << currentChar << " with count " << count << endl;
        } else {
            cerr << "Error: Invalid count found in the compressed file." << endl;
            inFile.close();
            outFile.close();
            return;
        }
    }

    inFile.close();
    outFile.close();

    cout << "Decompression completed." << endl;
}

int main() {
    string directory = "F:\\";  

    string inputFile = directory + "File.txt";  
    string compressedFile = directory + "File.txt.rle";  
    string decompressedFile = directory + "decompressed_File.txt";  

    writeFile(inputFile);

    compressFile(inputFile, compressedFile);
    cout << "File compressed to: " << compressedFile << endl;

    decompressFile(compressedFile, decompressedFile);
    cout << "File decompressed to: " << decompressedFile << endl;

    return 0;
}

