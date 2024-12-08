#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include <fstream>

using namespace std;

const string ClientsFiles = "FileClient.txt";
const string RegisterTransfer = "Transferfile.txt";
enum enMode {};
class clsBank : public clsperson
{
private:
    enum enMode { EmptyMode = 0, UpdateMode = 1, AddNewMode = 2 };
    enMode _Mode;
    string _AccountNumber;
    string _PinCode;
    float _AccountBalance;
    bool _MarkedForDelete = false;

    static clsBank _ConvertLinetoClientObject(string Line, string seprator = "#//#")
    {
        vector <string> vClients;
        vClients = clsString::Split(Line, seprator);

        return clsBank(enMode::UpdateMode, vClients[0], vClients[1], vClients[2], vClients[3], vClients[4], vClients[5], stod(vClients[6]));
    }
    static  vector <clsBank> _LoadClientsDataFromFile()
    {

        vector <clsBank> vClients;

        fstream MyFile;
        MyFile.open("FileClient.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;


            while (getline(MyFile, Line))
            {

                clsBank Client = _ConvertLinetoClientObject(Line);

                vClients.push_back(Client);
            }

            MyFile.close();

        }

        return vClients;

    }
    static clsBank _GetEmptyClientObject()
    {
        return clsBank(enMode::EmptyMode, "", "", "", "", "", "", 0);
    }
    static string _ConverClientObjectToLine(clsBank Client, string Seperator = "#//#")
    {

        string stClientRecord = "";
        stClientRecord += Client.FirstName + Seperator;
        stClientRecord += Client.LastName + Seperator;
        stClientRecord += Client.Email + Seperator;
        stClientRecord += Client.Phone + Seperator;
        stClientRecord += Client.AccountNumber() + Seperator;
        stClientRecord += Client.PinCode + Seperator;
        stClientRecord += to_string(Client.AccountBalance);

        return stClientRecord;

    }
    //static void _SaveCleintsDataToFile(vector <clsBank> vClients)
    //{

    //    fstream MyFile;
    //    MyFile.open("Clients.txt", ios::out);//overwrite

    //    string DataLine;

    //    if (MyFile.is_open())
    //    {

    //        for (clsBank C : vClients)
    //        {
    //            DataLine = _ConverClientObjectToLine(C);
    //            MyFile << DataLine << endl;

    //        }

    //        MyFile.close();

    //    }

    //}
    static void _SaveCleintsDataToFile(vector <clsBank> vClients)
    {

        fstream MyFile;
        MyFile.open("FileClient.txt", ios::out);//overwrite

        string DataLine;

        if (MyFile.is_open())
        {

            for (clsBank C : vClients)
            {
                if (C.MarkedForDeleted() == false)
                {
                    //we only write records that are not marked for delete.  
                    DataLine = _ConverClientObjectToLine(C);
                    MyFile << DataLine << endl;

                }

            }

            MyFile.close();

        }

    }
    void _Update()
    {
        vector <clsBank> vClients = _LoadClientsDataFromFile();
        for (clsBank& c : vClients)
        {
            if (c.AccountNumber() == AccountNumber())
            {
                c = *this;
                break;
            }
        }
        _SaveCleintsDataToFile(vClients);
    }
    void _AddNew()
    {
        _AddDataLineToFile(_ConverClientObjectToLine(*this));
    }
    void _AddDataLineToFile(string  stDataLine)
    {
        fstream MyFile;
        MyFile.open("FileClient.txt", ios::out | ios::app);

        if (MyFile.is_open())
        {

            MyFile << stDataLine << endl;

            MyFile.close();
        }

    }

    string _PrepareTransferLogRecord(clsBank Destantion, double Amount, string UserName,string space = "#//#")
    {
        string TransferLogRecord = "";
        TransferLogRecord += clsDate::GetSystemDateTimeString() + space;
        TransferLogRecord += AccountNumber() + space;
        TransferLogRecord += Destantion.AccountNumber() + space;
        TransferLogRecord += to_string(Amount) + space;
        TransferLogRecord += to_string(AccountBalance) + space;
        TransferLogRecord += to_string(Destantion.AccountBalance) + space;
        TransferLogRecord += UserName;
        return TransferLogRecord;
    }

    void _RegisterTransfer(clsBank Destantion, double Amount, string UserName)
    {
        string stDataLine = _PrepareTransferLogRecord(Destantion, Amount, UserName);
        fstream Myfile;
        Myfile.open("Transferfile.txt", ios::out | ios::app);
        if (Myfile.is_open())
        {
            Myfile << stDataLine << endl;
            Myfile.close();
        }

    }
    struct stTrnsferLogRecord;
    static stTrnsferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
    {
        stTrnsferLogRecord TrnsferLogRecord;

        vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
        TrnsferLogRecord.DateTime = vTrnsferLogRecordLine[0];
        TrnsferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
        TrnsferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
        TrnsferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
        TrnsferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
        TrnsferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
        TrnsferLogRecord.UserName = vTrnsferLogRecordLine[6];

        return TrnsferLogRecord;

    }

public:
    clsBank(enMode Mode, string FirstName, string LastName, string Email, string Phone, string AccountNumber, string PinCode, float AccountBalance)
        : clsperson(FirstName, LastName, Email, Phone)
    {
        _Mode = Mode;
        _AccountNumber = AccountNumber;
        _PinCode = PinCode;
        _AccountBalance = AccountBalance;
    }
    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }
    bool MarkedForDeleted()
    {
        return _MarkedForDelete;
    }
    string AccountNumber()
    {
        return _AccountNumber;
    }

    void SetPinCode(string PinCode)
    {
        _PinCode = PinCode;
    }

    string GetPinCode()
    {
        return _PinCode;
    }
    __declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

    void SetAccountBalance(float AccountBalance)
    {
        _AccountBalance = AccountBalance;
    }

    float GetAccountBalance()
    {
        return _AccountBalance;
    }
    __declspec(property(get = GetAccountBalance, put = SetAccountBalance)) float AccountBalance;

    void Print()
    {
        cout << "\nClient Card:";
        cout << "\n___________________";
        cout << "\nFirstName   : " << FirstName;
        cout << "\nLastName    : " << LastName;
        cout << "\nFull Name   : " << FullName();
        cout << "\nEmail       : " << Email;
        cout << "\nPhone       : " << Phone;
        cout << "\nAcc. Number : " << _AccountNumber;
        cout << "\nPassword    : " << _PinCode;
        cout << "\nBalance     : " << _AccountBalance;
        cout << "\n___________________\n";
    }

    static clsBank Find(string AccountNumber)
    {
        fstream MyFile;
        MyFile.open("FileClient.txt", ios::in);
        if (MyFile.is_open())
        {
            string Line;
            while (getline(MyFile, Line))
            {
                clsBank client = _ConvertLinetoClientObject(Line);
                if (client.AccountNumber() == AccountNumber)
                {
                    MyFile.close();
                    return client;
                }
            }
            MyFile.close();
        }
        return clsBank::_GetEmptyClientObject();
    }

    enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };

    enSaveResults Save()
    {

        switch (_Mode)
        {
        case enMode::EmptyMode:
        {
            if (IsEmpty())
            {

                return enSaveResults::svFaildEmptyObject;

            }

        }

        case enMode::UpdateMode:
        {


            _Update();

            return enSaveResults::svSucceeded;

            break;
        }

        case enMode::AddNewMode:
        {
            //This will add new record to file or database
            if (clsBank::IsClientExist(_AccountNumber))
            {
                return enSaveResults::svFaildAccountNumberExists;
            }
            else
            {
                _AddNew();

                //We need to set the mode to update after add new
                _Mode = enMode::UpdateMode;
                return enSaveResults::svSucceeded;
            }

            break;
        }
        }

    }
    bool Delete()
    {
        vector <clsBank> _vClients;
        _vClients = _LoadClientsDataFromFile();

        for (clsBank& C : _vClients)
        {
            if (C.AccountNumber() == _AccountNumber)
            {
                C._MarkedForDelete = true;
                break;
            }

        }

        _SaveCleintsDataToFile(_vClients);

        *this = _GetEmptyClientObject();

        return true;
    }
    static float GetTotalBalances()
    {
        vector <clsBank> vClients = clsBank::GetClientsList();

        double TotalBalances = 0;

        for (clsBank Client : vClients)
        {

            TotalBalances += Client.AccountBalance;
        }

        return TotalBalances;

    }
    static bool IsClientExist(string AccountNumber)
    {

        clsBank Client1 = clsBank::Find(AccountNumber);
        return (!Client1.IsEmpty());
    }
    static clsBank GetAddNewClientObject(string AccountNumber)
    {
        return clsBank(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
    }
    static vector <clsBank> GetClientsList()
    {
        return _LoadClientsDataFromFile();
    }

    void Deposit(double Amount)
    {
        _AccountBalance += Amount;
        Save();
    }

    bool Withdraw(double Amount)
    {
        if (Amount > _AccountBalance)
        {
            return false;
        }
        else
        {
            _AccountBalance -= Amount;
            Save();
        }
    }

    static double GetTotalBalancess()
    {
        vector <clsBank> vClients = clsBank::GetClientsList();

        double TotalBalances = 0;

        for (clsBank Client : vClients)
        {

            TotalBalances += Client.AccountBalance;
        }

        return TotalBalances;
    }
    bool Transfer(float Amount, clsBank& DestinationClient,string UserName)
    {
        if (Amount > AccountBalance)
        {
            return false;
        }

        Withdraw(Amount);
        DestinationClient.Deposit(Amount);
        _RegisterTransfer(DestinationClient, Amount, UserName);
        return true;
    }
    struct stTrnsferLogRecord
    {
        string DateTime;
        string SourceAccountNumber;
        string DestinationAccountNumber;
        float Amount;
        float srcBalanceAfter;
        float destBalanceAfter;
        string UserName;

    };
    static  vector <stTrnsferLogRecord> GetTransfersLogList()
    {
        vector <stTrnsferLogRecord> vTransferLogRecord;

        fstream MyFile;
        MyFile.open("TransfersLog.txt", ios::in);//read Mode

        if (MyFile.is_open())
        {

            string Line;

            stTrnsferLogRecord TransferRecord;

            while (getline(MyFile, Line))
            {

                TransferRecord = _ConvertTransferLogLineToRecord(Line);

                vTransferLogRecord.push_back(TransferRecord);

            }

            MyFile.close();

        }

        return vTransferLogRecord;

    }


    
};

