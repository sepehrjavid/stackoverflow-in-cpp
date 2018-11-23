//
// Created by navid-fkh on 11/21/18.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include "Logger.h"

using namespace std;

Logger::Logger() {}

Logger& Logger::getInstance() {
    static Logger lg;
    return lg;
}

void Logger::log(std::string lg) {
    FILE* datafil = fopen("datafile.txt","r");
    if(!datafil){
        FILE* datafile = fopen("datafile.txt","a");
        fseek(datafile, 0, SEEK_SET);
        fprintf(datafile,"%d", 1);
        fclose(datafile);
    }
    FILE* datafile = fopen("datafile.txt","r");
    int count;
    fseek(datafile, 0, SEEK_SET);
    fscanf(datafile,"%d",&count);
    fclose(datafile);
    FILE* datafile2 = fopen("datafile.txt","w");
    char s[] = "           ";
    sprintf(s,"log.%d.txt",count);
    fseek(datafile2, 0, SEEK_SET);
    fprintf(datafile2,"%d", ++count);
    fclose(datafile2);
    FILE* logfile = fopen(s,"a");
    fseek(logfile, 0, SEEK_SET);
    const char *clg = lg.c_str();
    fprintf(logfile,clg);
    fclose(logfile);
    this->logs.push_back(lg);
}


void Logger::printLogs() {
    for (const auto &log : logs) {
        std::cout << log << std::endl;
    }
}

std::vector<std::string>& Logger::getLogs() {
    return this->logs;
}

//fstream Logger::operator << (fstream &out, string str){
//    return out;
//}


