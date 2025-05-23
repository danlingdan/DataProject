#include <iostream>
#include <string>
#include <functional>
#include <thread> //�߳�
#include <chrono> //ʱ�����ʱ
#include <ctime>
#include <vector> //����
#include <limits>

void mainMenu() {
	std::cout << "�����˻�����ϵͳ" << std::endl;
	std::cout << "1. ��������" << std::endl;
	std::cout << "2. ������" << std::endl;
	std::cout << "3. ȡ�����" << std::endl;
	std::cout << "4. ���ÿ�����" << std::endl;
	std::cout << "5. ���ÿ�����" << std::endl;
	std::cout << "6. ��ѯ�˻�" << std::endl;
	std::cout << "7. �޸�����" << std::endl;
	std::cout << "8. �鿴������ʷ" << std::endl;
	std::cout << "9. �˻�ע��" << std::endl;
	std::cout << "10. ���¼����˻�" << std::endl;
	std::cout << "11. ��ʾ�����˻�" << std::endl;
	std::cout << "0. �˳�" << std::endl;
}

//�ṹ����Ա
struct Person {
	std::string name;
	std::string address;
	std::string phoneNumber;
	short int age =0;
};

// ���п�����
class BankCard {
private:
	Person owner; //�ֿ���
	unsigned long long cardNumber;
	std::string expirationDate;
	std::string cvv;
	unsigned int password;
	bool active; // �Ƿ񼤻�
	std::vector<std::string> transactionHistory; // ���׼�¼
protected:
	// ��ӽ��׼�¼
	void addTransaction(const std::string& transaction) {
		// ��ȡ��ǰʱ����Ϊ����ʱ��
		auto now = std::chrono::system_clock::now();
		std::time_t current_time = std::chrono::system_clock::to_time_t(now);
		std::tm timeinfo;
		localtime_s(&timeinfo, &current_time);
		char timeStr[20];
		std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

		std::string record = std::string(timeStr) + " - " + transaction;
		transactionHistory.push_back(record);
	}
public:
	BankCard(std::string expDate, std::string cvvCode, Person owner, unsigned int pass)
		: owner(owner), active(true) {
		expirationDate = generateExpirationDate();
		cvv = generateCVV();
		cardNumber = generateCardNumber();
		password = pass;
		addTransaction("�����ɹ�");
	}
	unsigned long long getCardNumber() {
		return cardNumber;
	}
	std::string getOwnerName() {
		return owner.name;
	}
	std::string getOwnerAddress() {
		return owner.address;
	}
	std::string getOwnerPhoneNumber() {
		return owner.phoneNumber;
	}
	short int getOwnerAge() {
		return owner.age;
	}
	std::string getExpirationDate() {
		return expirationDate;
	}
	std::string getCVV() {
		return cvv;
	}
	// ��֤����
	bool verifyPassword(unsigned int inputPassword) const {
		return password == inputPassword;
	}
	// �޸�����
	bool changePassword(unsigned int oldPassword, unsigned int newPassword) {
		if (verifyPassword(oldPassword)) {
			password = newPassword;
			addTransaction("�����޸ĳɹ�");
			return true;
		}
		return false;
	}
	// ע���˻�
	bool closeAccount(unsigned int inputPassword) {
		if (verifyPassword(inputPassword)) {
			active = false;
			addTransaction("�˻�ע��");
			return true;
		}
		return false;
	}
	// �˻��Ƿ񼤻�
	bool isActive() const {
		return active;
	}
	// ���¼����˻�
	bool reactivateAccount(unsigned int inputPassword) {
		if (verifyPassword(inputPassword)) {
			active = true;
			addTransaction("�˻����¼���");
			return true;
		}
		return false;
	}
	// ��ʾ������ʷ
	void showTransactionHistory() const {
		std::cout << "\n������ʷ��¼:" << std::endl;
		if (transactionHistory.empty()) {
			std::cout << "���޽��׼�¼" << std::endl;
			return;
		}

		for (const auto& transaction : transactionHistory) {
			std::cout << transaction << std::endl;
		}
	}
    // ����Ψһ16λ�����п���(ʹ��ʱ�䡢�̵߳Ĺ�ϣʵ��)
	unsigned long long generateCardNumber() {
		static int counter = 0;
		// ʹ�õ�ǰʱ����Ϊ����
		auto now = std::chrono::high_resolution_clock::now();
		auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

		// ��϶��Ԫ����ȷ��Ψһ��
		std::string uniqueStr =
			std::to_string(timestamp) +
			std::to_string(counter++) +
			std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

		// ����16λ����
		// ��1λ: ���п����ͣ����ÿ���4��ͷ����ǿ���6��ͷ
		std::string cardNumber = "4"; // Ĭ��Ϊ���ÿ�ǰ׺

		// ����14λ�ӹ�ϣֵ����ȡ
		std::size_t hashValue = std::hash<std::string>{}(uniqueStr);
		std::string hashStr = std::to_string(hashValue);

		// ȷ��������14λ���ֿ���
		while (hashStr.length() < 14) {
			hashStr += std::to_string(counter++);
		}

		// ȡǰ14λ
		cardNumber += hashStr.substr(0, 14);

		// ���㲢���У��λ����16λ��
		int sum = 0;
		for (int i = 0; i < 15; i++) {
			int digit = cardNumber[i] - '0';
			if (i % 2 == 0) { // ż��λ����0��ʼ������
				digit *= 2;
				if (digit > 9) {
					digit -= 9;
				}
			}
			sum += digit;
		}
		int checkDigit = (10 - (sum % 10)) % 10;
		cardNumber += std::to_string(checkDigit);

		// ת��Ϊ���ַ���
		unsigned long long result = std::stoull(cardNumber);
		return result;
	}
	// ����Ψһ��CVV��
	std::string generateCVV() {
		static int counter = 0;
		// ʹ�õ�ǰʱ����Ϊ����
		auto now = std::chrono::high_resolution_clock::now();
		auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		// ��϶��Ԫ����ȷ��Ψһ��
		std::string uniqueStr =
			std::to_string(timestamp) +
			std::to_string(counter++) +
			std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));
		// ʹ�ñ�׼��ϣ��������ΨһID
		return std::to_string(std::hash<std::string>{}(uniqueStr) % 1000); // CVVΪ3λ��
	}
	//����ʱ�� - ȫ��������
	std::string generateExpirationDate() {
		// ��ȡ��ǰʱ��
		std::tm timeinfo;
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_s(&timeinfo, &now);

		// ����ԭʼ������Ϣ
		int original_day = timeinfo.tm_mday;
		int original_month = timeinfo.tm_mon;
		int original_year = timeinfo.tm_year;

		// ����5��
		timeinfo.tm_year += 5;

		// ����·ݺ����ڵ���Ч��
		int year = timeinfo.tm_year + 1900;

		// �����·�ȷ���������
		int max_days = 31; // Ĭ��Ϊ31��

		if (timeinfo.tm_mon == 1) { // ����
			// �������
			bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
			max_days = isLeapYear ? 29 : 28;
		}
		else if (timeinfo.tm_mon == 3 || timeinfo.tm_mon == 5 ||
			timeinfo.tm_mon == 8 || timeinfo.tm_mon == 10) {
			// 4, 6, 9, 11��
			max_days = 30;
		}

		// ��������������������
		if (timeinfo.tm_mday > max_days) {
			timeinfo.tm_mday = max_days;
		}

		// ��׼������
		std::time_t expTime = std::mktime(&timeinfo);
		if (expTime == -1) {
			// �����������ʹ�ü򵥵��ɿ��ķ���
			// ������Ϊ��ǰʱ��
			localtime_s(&timeinfo, &now);

			// �ȼ�4�꣨�ܿ��������⣩
			timeinfo.tm_year += 4;
			std::mktime(&timeinfo); // ��׼��

			// �ټ�1��
			timeinfo.tm_year += 1;
			std::mktime(&timeinfo); // ��׼��
		}

		// ��ʽ������
		char buffer[11];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
		return std::string(buffer);
	}

	virtual void input() {
		std::cout << "������ֿ�������: ";
		std::getline(std::cin, owner.name);
		std::cout << "������ֿ��˵�ַ: ";
		std::getline(std::cin, owner.address);
		std::cout << "������ֿ��˵绰: ";
		std::getline(std::cin, owner.phoneNumber);
		std::cout << "������ֿ�������: ";
		std::cin >> owner.age;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// ��������
		unsigned int newPassword, confirmPassword;
		do {
			std::cout << "������6λ��������: ";
			std::cin >> newPassword;
			std::cout << "���ٴ���������: ";
			std::cin >> confirmPassword;

			if (newPassword != confirmPassword) {
				std::cout << "������������벻һ�£����������룡" << std::endl;
			}
		} while (newPassword != confirmPassword);

		password = newPassword;

		cardNumber = generateCardNumber();
		expirationDate = generateExpirationDate();
		cvv = generateCVV();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	virtual void display() const {
		std::cout << "���п���Ϣ:" << std::endl;
		std::cout << "�ֿ�������: " << owner.name << std::endl;
		std::cout << "�ֿ��˵�ַ: " << owner.address << std::endl;
		std::cout << "�ֿ��˵绰: " << owner.phoneNumber << std::endl;
		std::cout << "�ֿ�������: " << owner.age << std::endl;
		std::cout << "���п���: " << cardNumber << std::endl;
		std::cout << "����ʱ��: " << expirationDate << std::endl;
		std::cout << "CVV��: " << cvv << std::endl;
		std::cout << "��Ƭ״̬: " << (active ? "����" : "��ע��") << std::endl;
	}
};

// ���ÿ���
class CreditCard : public BankCard {
private:
	double creditLimit;
	double currentBalance;
	double outstandingBalance; // δ������

public:
	CreditCard(std::string expDate, std::string cvvCode, Person owner, unsigned int pass, double limit)
		: BankCard(expDate, cvvCode, owner, pass), creditLimit(limit), currentBalance(0.0), outstandingBalance(0.0) {
	}
	// ����
	bool charge(double amount) {
		if (!isActive()) {
			std::cout << "�˻���ע�����޷�����" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "���ѽ�����Ϊ����" << std::endl;
			return false;
		}

		if (currentBalance + amount > creditLimit) {
			std::cout << "�������ö�ȣ�����ʧ��" << std::endl;
			return false;
		}

		currentBalance += amount;
		outstandingBalance += amount;
		std::string transaction = "���� $" + std::to_string(amount) + "����ǰ���ö��: $" + std::to_string(currentBalance);
		addTransaction(transaction);
		return true;
	}

	// ����
	bool makePayment(double amount) {
		if (!isActive()) {
			std::cout << "�˻���ע�����޷�����" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "���������Ϊ����" << std::endl;
			return false;
		}

		if (amount > outstandingBalance) {
			amount = outstandingBalance; // ��������Ƿ�ֻ��Ƿ����
		}

		outstandingBalance -= amount;
		currentBalance -= amount;

		std::string transaction = "���� $" + std::to_string(amount) + "��ʣ��Ƿ��: $" + std::to_string(outstandingBalance);
		addTransaction(transaction);
		return true;
	}

	// ��ȡ���ö��
	double getAvailableCredit() const {
		return creditLimit - currentBalance;
	}

	// ��ȡ��ʹ�ö��
	double getCurrentBalance() const {
		return currentBalance;
	}

	// ��ȡδ������
	double getOutstandingBalance() const {
		return outstandingBalance;
	}

	void input() override {
		BankCard::input();
		std::cout << "���������ö��: $";
		std::cin >> creditLimit;
	}

	// ��ʾ���ÿ���Ϣ
	void display() const override {
		std::cout << "���ÿ���Ϣ:" << std::endl;
		BankCard::display();
		std::cout << "���ö��: $" << creditLimit << std::endl;
		std::cout << "���ö��: $" << currentBalance << std::endl;
		std::cout << "ʣ����ö��: $" << getAvailableCredit() << std::endl;
		std::cout << "δ������: $" << outstandingBalance << std::endl;
	}
};	

// ��ǿ���
class DebitCard : public BankCard {
private:
	double balance;
public:
	DebitCard(std::string expDate, std::string cvvCode, Person owner, unsigned int pass)
		: BankCard(expDate, cvvCode, owner, pass), balance(0.0) {
	}
	// ���
	bool deposit(double amount) {
		if (!isActive()) {
			std::cout << "�˻���ע�����޷����" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "��������Ϊ����" << std::endl;
			return false;
		}

		balance += amount;
		std::string transaction = "��� $" + std::to_string(amount) + " �ɹ�����ǰ���: $" + std::to_string(balance);
		addTransaction(transaction);
		return true;
	}
	// ȡ��
	bool withdraw(double amount, unsigned int inputPassword) {
		if (!isActive()) {
			std::cout << "�˻���ע�����޷�ȡ��" << std::endl;
			return false;
		}

		if (!verifyPassword(inputPassword)) {
			std::cout << "�������ȡ��ʧ��" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "ȡ�������Ϊ����" << std::endl;
			return false;
		}

		if (amount > balance) {
			std::cout << "���㣬ȡ��ʧ��" << std::endl;
			return false;
		}

		balance -= amount;
		std::string transaction = "ȡ�� $" + std::to_string(amount) + " �ɹ�����ǰ���: $" + std::to_string(balance);
		addTransaction(transaction);
		return true;
	}
	// ��ѯ���
	double getBalance() const {
		return balance;
	}

	void input() override {
		BankCard::input();
		double initialDeposit = 0.0;
		std::cout << "�������ʼ�����: $";
		std::cin >> initialDeposit;
		if (initialDeposit > 0) {
			balance = initialDeposit;
			std::string transaction = "��ʼ��� $" + std::to_string(initialDeposit);
			addTransaction(transaction);
		}
	}

	// ��ʾ��ǿ���Ϣ
	void display() const override {
		std::cout << "��ǿ���Ϣ:" << std::endl;
		BankCard::display();
		std::cout << "��ǰ���: $" << balance << std::endl;
	}
};

// ���п�������
class CardManager {
private:
	std::vector<BankCard*> allCards;
	std::vector<CreditCard*> creditCards;
	std::vector<DebitCard*> debitCards;

public:
	// ������п�
	void addCard(BankCard* card) {
		allCards.push_back(card);

		// ��鿨Ƭ���Ͳ���ӵ���Ӧ������
		CreditCard* creditCard = dynamic_cast<CreditCard*>(card);
		if (creditCard) {
			creditCards.push_back(creditCard);
			return;
		}

		DebitCard* debitCard = dynamic_cast<DebitCard*>(card);
		if (debitCard) {
			debitCards.push_back(debitCard);
		}
	}

	// ��ȡͳ����Ϣ
	unsigned int getTotalCardCount() const {
		return allCards.size();
	}
	unsigned int getCreditCardCount() const {
		return creditCards.size();
	}
	unsigned int getDebitCardCount() const {
		return debitCards.size();
	}

	// ���ݿ��Ų��ҿ�Ƭ
	BankCard* findCardByNumber(unsigned long long cardNumber) {
		for (auto card : allCards) {
			if (card->getCardNumber() == cardNumber) {
				return card;
			}
		}
		return nullptr;
	}

	// ��ʾ���п�Ƭ��Ϣ
	void displayAllCards() const {
		std::cout << "���п�����: " << getTotalCardCount() << std::endl;
		std::cout << "���ÿ�����: " << getCreditCardCount() << std::endl;
		std::cout << "��ǿ�����: " << getDebitCardCount() << std::endl;

		std::cout << "\n���п�Ƭ��Ϣ:" << std::endl;
		for (size_t i = 0; i < allCards.size(); i++) {
			std::cout << "��Ƭ #" << (i + 1) << ":" << std::endl;
			if (dynamic_cast<CreditCard*>(allCards[i])) {
				dynamic_cast<CreditCard*>(allCards[i])->display();
			}
			else if (dynamic_cast<DebitCard*>(allCards[i])) {
				dynamic_cast<DebitCard*>(allCards[i])->display();
			}
			std::cout << "---------------------------" << std::endl;
		}
	}
	// ������п��ֶ�����
	void addCardManually() {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺����

		Person newOwner;
		std::cout << "\n==== ��������п� ====" << std::endl;

		// ѡ��Ƭ����
		int cardType;
		std::cout << "��ѡ�����п����ͣ�" << std::endl;
		std::cout << "1. ���ÿ�" << std::endl;
		std::cout << "2. ��ǿ�" << std::endl;
		std::cout << "������ѡ�� (1-2): ";
		std::cin >> cardType;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺����

		if (cardType != 1 && cardType != 2) {
			std::cout << "��Ч��ѡ��Ĭ�ϴ�����ǿ���" << std::endl;
			cardType = 2;
		}

		BankCard* newCard = nullptr;

		if (cardType == 1) {
			// �������ÿ�
			CreditCard* creditCard = new CreditCard("", "", newOwner, 123456, 0.0);
			creditCard->input();
			newCard = creditCard;
			std::cout << "\n���ÿ������ɹ���" << std::endl;
		}
		else {
			// ������ǿ�
			DebitCard* debitCard = new DebitCard("", "", newOwner,123456);
			debitCard->input();
			newCard = debitCard;
			std::cout << "\n��ǿ������ɹ���" << std::endl;
		}

		// ��ӵ�������
		addCard(newCard);
	}
	// �� CardManager ���������������
	void searchCard() {
		int searchType;
		std::cout << "\n==== �������п� ====" << std::endl;
		std::cout << "1. �����Ų���" << std::endl;
		std::cout << "2. ���ֿ�����������" << std::endl;
		std::cout << "��������ҷ�ʽ (1-2): ";
		std::cin >> searchType;

		if (searchType == 1) {
			unsigned long long cardNumber;
			std::cout << "�����뿨��: ";
			std::cin >> cardNumber;

			BankCard* card = findCardByNumber(cardNumber);
			if (card) {
				std::cout << "\n�ҵ�ƥ��Ŀ�Ƭ��" << std::endl;
				if (dynamic_cast<CreditCard*>(card)) {
					dynamic_cast<CreditCard*>(card)->display();
				}
				else if (dynamic_cast<DebitCard*>(card)) {
					dynamic_cast<DebitCard*>(card)->display();
				}
			}
			else {
				std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			}
		}
		else if (searchType == 2) {
			std::string name;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺����
			std::cout << "������ֿ�������: ";
			std::getline(std::cin, name);

			bool found = false;
			for (auto card : allCards) {
				if (card->getOwnerName() == name) {
					found = true;
					std::cout << "\n�ҵ�ƥ��Ŀ�Ƭ��" << std::endl;
					if (dynamic_cast<CreditCard*>(card)) {
						dynamic_cast<CreditCard*>(card)->display();
					}
					else if (dynamic_cast<DebitCard*>(card)) {
						dynamic_cast<DebitCard*>(card)->display();
					}
					std::cout << "---------------------------" << std::endl;
				}
			}

			if (!found) {
				std::cout << "δ�ҵ��ֿ���Ϊ " << name << " �����п���" << std::endl;
			}
		}
		else {
			std::cout << "��Ч��ѡ��" << std::endl;
		}
	}
	// ������
	void depositToAccount() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== ������ ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		DebitCard* debitCard = dynamic_cast<DebitCard*>(card);
		if (!debitCard) {
			std::cout << "�ÿ����ǽ�ǿ����޷���" << std::endl;
			return;
		}

		std::cout << "����������: $";
		std::cin >> amount;

		if (debitCard->deposit(amount)) {
			std::cout << "���ɹ�! ��ǰ���: $" << debitCard->getBalance() << std::endl;
		}
		else {
			std::cout << "���ʧ��!" << std::endl;
		}
	}

	// ȡ�����
	void withdrawFromAccount() {
		unsigned long long cardNumber;
		double amount;
		unsigned int password;

		std::cout << "\n==== ȡ����� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		DebitCard* debitCard = dynamic_cast<DebitCard*>(card);
		if (!debitCard) {
			std::cout << "�ÿ����ǽ�ǿ����޷�ȡ�" << std::endl;
			return;
		}

		std::cout << "������ȡ����: $";
		std::cin >> amount;
		std::cout << "����������: ";
		std::cin >> password;

		if (debitCard->withdraw(amount, password)) {
			std::cout << "ȡ��ɹ�! ��ǰ���: $" << debitCard->getBalance() << std::endl;
		}
		else {
			std::cout << "ȡ��ʧ��!" << std::endl;
		}
	}

	// ���ÿ�����
	void chargeToCreditCard() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== ���ÿ����� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		CreditCard* creditCard = dynamic_cast<CreditCard*>(card);
		if (!creditCard) {
			std::cout << "�ÿ��������ÿ����޷��������ѡ�" << std::endl;
			return;
		}

		std::cout << "���������ѽ��: $";
		std::cin >> amount;

		if (creditCard->charge(amount)) {
			std::cout << "���ѳɹ�! ʣ����ö��: $" << creditCard->getAvailableCredit() << std::endl;
		}
		else {
			std::cout << "����ʧ��!" << std::endl;
		}
	}

	// ���ÿ�����
	void makePaymentToCreditCard() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== ���ÿ����� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		CreditCard* creditCard = dynamic_cast<CreditCard*>(card);
		if (!creditCard) {
			std::cout << "�ÿ��������ÿ����޷����л��" << std::endl;
			return;
		}

		std::cout << "��ǰδ������: $" << creditCard->getOutstandingBalance() << std::endl;
		std::cout << "�����뻹����: $";
		std::cin >> amount;

		if (creditCard->makePayment(amount)) {
			std::cout << "����ɹ�! ʣ��δ������: $" << creditCard->getOutstandingBalance() << std::endl;
		}
		else {
			std::cout << "����ʧ��!" << std::endl;
		}
	}

	// �޸�����
	void changeCardPassword() {
		unsigned long long cardNumber;
		unsigned int oldPassword, newPassword, confirmPassword;

		std::cout << "\n==== �޸����� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		std::cout << "������ԭ����: ";
		std::cin >> oldPassword;

		do {
			std::cout << "������������: ";
			std::cin >> newPassword;
			std::cout << "��ȷ��������: ";
			std::cin >> confirmPassword;

			if (newPassword != confirmPassword) {
				std::cout << "������������벻һ�£����������룡" << std::endl;
			}
		} while (newPassword != confirmPassword);

		if (card->changePassword(oldPassword, newPassword)) {
			std::cout << "�����޸ĳɹ�!" << std::endl;
		}
		else {
			std::cout << "�����޸�ʧ�ܣ�ԭ�������!" << std::endl;
		}
	}

	// ע���˻�
	void closeAccount() {
		unsigned long long cardNumber;
		unsigned int password;

		std::cout << "\n==== ע���˻� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		std::cout << "����������: ";
		std::cin >> password;

		if (card->closeAccount(password)) {
			std::cout << "�˻�ע���ɹ�!" << std::endl;
		}
		else {
			std::cout << "�˻�ע��ʧ�ܣ��������!" << std::endl;
		}
	}

	// ���¼����˻�
	void reactivateAccount() {
		unsigned long long cardNumber;
		unsigned int password;

		std::cout << "\n==== ���¼����˻� ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		if (card->isActive()) {
			std::cout << "���˻��Ѿ��Ǽ���״̬���������¼��" << std::endl;
			return;
		}

		std::cout << "����������: ";
		std::cin >> password;

		if (card->reactivateAccount(password)) {
			std::cout << "�˻����¼���ɹ�!" << std::endl;
		}
		else {
			std::cout << "�˻����¼���ʧ�ܣ��������!" << std::endl;
		}
	}

	// �鿴������ʷ
	void viewTransactionHistory() {
		unsigned long long cardNumber;

		std::cout << "\n==== �鿴������ʷ ====" << std::endl;
		std::cout << "�����뿨��: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "δ�ҵ�����Ϊ " << cardNumber << " �����п���" << std::endl;
			return;
		}

		card->showTransactionHistory();
	}
	// �����������ͷ��ڴ�
	~CardManager() {
		for (auto card : allCards) {
			delete card;
		}
		allCards.clear();
		creditCards.clear();
		debitCards.clear();
	}
};

int main() {
	// ������Ƭ������
	CardManager cardManager;
	int choice = -1;

	while (choice != 0) {
		mainMenu();
		std::cout << "������ѡ�� (0-11): ";
		std::cin >> choice;

		switch (choice) {
		case 1: // ��������
			cardManager.addCardManually();
			break;
		case 2: // ������
			cardManager.depositToAccount();
			break;
		case 3: // ȡ�����
			cardManager.withdrawFromAccount();
			break;
		case 4: // ���ÿ�����
			cardManager.chargeToCreditCard();
			break;
		case 5: // ���ÿ�����
			cardManager.makePaymentToCreditCard();
			break;
		case 6: // ��ѯ�˻�
			cardManager.searchCard();
			break;
		case 7: // �޸�����
			cardManager.changeCardPassword();
			break;
		case 8: // �鿴������ʷ
			cardManager.viewTransactionHistory();
			break;
		case 9: // �˻�ע��
			cardManager.closeAccount();
			break;
		case 10: // ���¼����˻�
			cardManager.reactivateAccount();
			break;
		case 11: // ��ʾ�����˻�
			cardManager.displayAllCards();
			break;
		case 0:
			std::cout << "��лʹ�������˻�����ϵͳ���ټ���" << std::endl;
			break;
		default:
			std::cout << "��Ч��ѡ�������ԡ�" << std::endl;
			break;
		}

		// ��ÿ�β�������ͣ
		if (choice != 0) {
			std::cout << "\n�� Enter ������...";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
			system("cls"); // ������Windowsϵͳ
		}
	}

	return 0;
}