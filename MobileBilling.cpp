/* In this exercise we want to design a solution for Mobile Billing . 
 * Our version of the Mobile Billing contains collection of customer details.
 *
 * Every customer has an id, name, number, balance, callDuration etc.
 * Assume suitable types for each data member
 * 	- id: <some unique id>
 * 	- name: "Test user"
 * 	- phoneNumber: "9800012345"
 *  - balance : 500 
 *  - totalCallDuration : 268
 *
 * Customers are classified as Prepaid,Postpaid users with some common operations,
 * e.g. credit, getBalance will be common for both. 
 * Customer id must be unique within Prepaid, Postpaid category but need not be 
 * unique across both the categories. Phone numbers must be unique across both the categories.
 * For Prepaid customers recharge is considered as credit operation
 * For postpaid customers billPay is considered as credit operation.
 * For both categories, outstanding amount is considered as balance.
 */
#include <iostream>
#include <string>
#include<vector>
#include<unordered_map>
#include <mutex>
#include <thread>
using namespace std;
const int maxLimitBalance = 500;
const int maxLimitCall = 500;
enum cmrType
{   prePaid = 1,
    postPaid =2
};
enum queryType
{   maxBalance= 1,
    nameStart =2,
    callDuration =3
};
class Customer
{
	/* attributes */
	protected:

    int ID;

public:
	static unordered_map<string,int> balance_pre;
	static unordered_map<string,int> balance_post;
    Customer()= default;
    Customer(int id_val):ID(id_val){}
    Customer(const Customer&)= delete;
    Customer& operator=(const Customer&)= delete;
    void credit (cmrType type,string number_to,int amount);
	int enquireBalance (string number_to);
	virtual void setBanalce(int amount)
	{
	     cout<<"Base class"<<endl;
	}
	virtual int getBanalce()
	{
	    cout<<"Base class"<<endl;
		return 0;
	}
	virtual void setcall(long call_time)
	{
	    cout<<"Base class"<<endl;
	}
	virtual long getcall()
	{
	   cout<<"Base class"<<endl;
	   return 0;
	}
	virtual string getName()
	{
	    cout<<"Base class"<<endl;
	}
	virtual string getNumber()
	{
	    cout<<"Base class"<<endl;
		return "";
	}
	virtual int getID()
	{
	    cout<<"Base class"<<endl;
		return 0;
	}
	static vector<string> getPostNumbers()
	{
		vector<string> numbers;
		
		for(auto it = balance_post.begin();it!=balance_post.end();it++)
		{
			if(it->second>=maxLimitBalance)
				numbers.push_back(it->first);
		}
		return numbers;
	}
	static vector<string> getPreNumbers()
	{
		vector<string> numbers;
		
		for(auto it = balance_pre.begin();it!=balance_pre.end();it++)
		{
			if(it->second>=maxLimitBalance)
				numbers.push_back(it->first);
		}
		return numbers;
	}

    /* fill in any other applicable members*/
    ~Customer()= default;
};
	 unordered_map<string,int> Customer::balance_pre;
	 unordered_map<string,int> Customer::balance_post;
void Customer::credit(cmrType type,string number,int amount)
{
	if(prePaid==type)
	{
		balance_pre.insert({number,amount});

	}
	else if(postPaid == type)
	{
		balance_post.insert({number,amount});
	}
}	

class PrepaidCustomer: public Customer
{
	string cus_number;
	int cus_balance;
    long totalCallDuration;
	string name;
    unordered_map<string,long> call; 
public:
    PrepaidCustomer()= default;
    PrepaidCustomer(int id_val,string number_val,string name_val):Customer(id_val),cus_number(number_val),name(name_val),cus_balance(0),totalCallDuration(0){}
    // void  recharge(string number,int amount);
    ~PrepaidCustomer() = default;
		void setBanalce(int amount)
	{
	    cus_balance = amount;
	}
	int getBanalce()
	{
	    return cus_balance;
	}
	void setcall(long call_time)
	{
	    totalCallDuration+=call_time;
	}
	long getcall()
	{
	    return totalCallDuration;
	}
	string getName()
	{
	    return name;
	}
	string getNumber()
	{
	    return cus_number;
	}
	int getID()
	{
	    return ID;
	}
};

class PostpaidCustomer: public Customer
{
   /*fill in*/
    string cus_number;
	int cus_balance;
    long totalCallDuration;
	string name;
    
	unordered_map<string,long> call;
public:
    PostpaidCustomer()= default;
    PostpaidCustomer(int id_val,string number_val,string name_val):Customer(id_val),cus_number(number_val),name(name_val),cus_balance(0),totalCallDuration(0){}
	//void billPay(string number,int amount);
    ~PostpaidCustomer()=default;
		void setBanalce(int amount)
	{
	    cus_balance = amount;
	}
	int getBanalce()
	{
	    return cus_balance;
	}
	void setcall(long call_time)
	{
	    totalCallDuration+=call_time;
	}
	long getcall()
	{
	    return totalCallDuration;
	}
	string getName()
	{
	    return name;
	}
	string getNumber()
	{
	    return cus_number;
	}
	int getID()
	{
	    return ID;
	}
};

class MobileBilling
{
public:
    void addCustomer(cmrType type, int id,string number,string name);
	void deleteCustomer(string number);
    vector<Customer*>searchById (int Id);
	vector<Customer*>searchByName(string name);
	Customer* searchByNumber(string number);
	long getAveragePostPaidCallDuration (string number);
    vector<Customer*> queryPostpaidCustomers (queryType type_val);
	int getAveragePrePaidBalance (string number);
    vector<Customer*> queryPrepaidCustomers (queryType type_val);

    void updateBalance(string number, int balance);
	void payBill(string number, int balance);
	void rechrage(string number, int balance);
	
	static MobileBilling* getIntance()
	{
		std::call_once(initInstanceFlag, &MobileBilling::InitMobileBilling);
		return instance;
	}
	static void InitMobileBilling(){
    instance= new MobileBilling;
  }
private:
	MobileBilling()=default;
	~MobileBilling()=default;
    MobileBilling(const MobileBilling&)= delete;
    MobileBilling& operator=(const MobileBilling&)= delete;
	static MobileBilling* instance;
	static std::once_flag initInstanceFlag;
	/*fill in */
    /* consider suitable container(s) for collection of customers */
    Customer* customerTyp;
    unordered_map<string,Customer*> numbers;
    unordered_multimap<string,Customer*> names;
    unordered_multimap<int,Customer*> IDs;

};
	 MobileBilling* MobileBilling::instance=nullptr;
	 std::once_flag MobileBilling::initInstanceFlag;
void MobileBilling::addCustomer(cmrType type,int id,string number,string name)
{
    if(1==type)
    {
        customerTyp = new PrepaidCustomer(id,number,name);
        numbers.insert({ number, customerTyp});
        IDs.insert({ id, customerTyp});
        names.insert({ name, customerTyp});
    }
    else if(2==type)
    {
        customerTyp = new PostpaidCustomer(id,number,name);
        numbers.insert({ number, customerTyp});
        IDs.insert({ id, customerTyp});
        names.insert({ name, customerTyp});
    }
}
Customer* MobileBilling::searchByNumber(string num)
{
    unordered_map<string,Customer*>::const_iterator got = numbers.find(num);
    if ( got == numbers.end() )
    {
        cout << "not found"<<endl;
        return nullptr;
    }
    else
    return got->second;
}
vector<Customer*> MobileBilling::searchById(int id)
{
    vector<Customer*> tmp_vec;
    auto it1 = IDs.equal_range(id);
    for (auto it= it1.first;it != it1.second;it++)
    {
        tmp_vec.push_back(it->second);
    }
    return tmp_vec;
}

vector<Customer*> MobileBilling::searchByName(string name)
{
    vector<Customer*> tmp_vec;
    auto it1 = names.equal_range(name);
    for (auto it= it1.first;it != it1.second;it++)
    {
        tmp_vec.push_back(it->second);
    }
    return tmp_vec;
}
void MobileBilling::updateBalance(string number, int balance)
{
	Customer* num = searchByNumber(number);
    unordered_map<string,Customer*>::iterator got = numbers.find(number);
    if (got == numbers.end() )
    {
        cout << "not found"<<endl;
    }
    else
	{
		got->second->setBanalce(balance);
	}
	int id_val=num->getID();
	auto it1 = IDs.equal_range(id_val);
    for (auto it= it1.first;it != it1.second;it++)
    {
       if(it->second->getNumber() == number )
		it->second->setBanalce(balance);
    }
    string name_val=num->getName();
    auto it2 = names.equal_range(name_val);
    for (auto it= it2.first;it != it2.second;it++)
    {
       if(it->second->getNumber()  == number )
		it->second->setBanalce(balance);
    }
}
vector<Customer*> MobileBilling::queryPrepaidCustomers(queryType type)
{
	vector<Customer*> cus_data;
	vector<string> number;
	if(1==type)
	{
		number = Customer::getPreNumbers();
	}
	for(auto num:number)
	{
		cus_data.push_back(searchByNumber(num));
	}
	return cus_data;
}
vector<Customer*> MobileBilling::queryPostpaidCustomers(queryType type)
{
	vector<Customer*> cus_data;
	vector<string> number;
	if(2==type)
	{
		number = Customer::getPostNumbers();
	}
	for(auto num:number)
	{
		cus_data.push_back(searchByNumber(num));
	}
	return cus_data;
}
 void MobileBilling::payBill(string number,int amount)
 {
	 Customer* num = searchByNumber(number);
	 cmrType type =postPaid;
	 num->credit(type,number,amount);
 }
  void MobileBilling::rechrage(string number,int amount)
 {
	 Customer* num = searchByNumber(number);
	 cmrType type =prePaid;
	 num->credit(type,number,amount);
 }
int main()
{
    MobileBilling *bill_obj = MobileBilling::getIntance();
	int type;
	int id,balance =0;
	string number, name;
	cout<<"Customer type: ";
	cin>>type;
	cout<<"Customer name: ";
	cin>>name;
	cout<<"Customer number: ";
	cin>>number;
    bill_obj->addCustomer(static_cast<cmrType>(type),id,number,name);
	cout<<"search number: ";
	cin>>number;
    Customer* cus_numbers = bill_obj->searchByNumber(number);
	cout<<"search name: ";
	cin>>number;
    vector<Customer*> v_names=bill_obj->searchByName(name);
	cout<<"search id: ";
	cin>>id;
    vector<Customer*> v_Ids=bill_obj->searchById(id);
	cout<<"Prepaid bill number: ";
	cin>>number;
	cout<<"Prepaid bill amount: ";
	cin>>balance;
    bill_obj->payBill(number,balance);
	bill_obj->updateBalance(number,balance);
	cout<<"Postpaid recharge number: ";
	cin>>number;
	cout<<"Postpaid recharge amount: ";
	cin>>balance;
    bill_obj->rechrage(number,balance);
    return 0;
}