#include <iostream>
#include <string>
#include <functional>
#include <thread> //线程
#include <chrono> //时间戳计时
#include <ctime>
#include <vector> //容器
#include <limits>

void mainMenu() {
	std::cout << "银行账户管理系统" << std::endl;
	std::cout << "1. 开户管理" << std::endl;
	std::cout << "2. 存款管理" << std::endl;
	std::cout << "3. 取款管理" << std::endl;
	std::cout << "4. 信用卡消费" << std::endl;
	std::cout << "5. 信用卡还款" << std::endl;
	std::cout << "6. 查询账户" << std::endl;
	std::cout << "7. 修改密码" << std::endl;
	std::cout << "8. 查看交易历史" << std::endl;
	std::cout << "9. 账户注销" << std::endl;
	std::cout << "10. 重新激活账户" << std::endl;
	std::cout << "11. 显示所有账户" << std::endl;
	std::cout << "0. 退出" << std::endl;
}

//结构体人员
struct Person {
	std::string name;
	std::string address;
	std::string phoneNumber;
	short int age =0;
};

// 银行卡基类
class BankCard {
private:
	Person owner; //持卡人
	unsigned long long cardNumber;
	std::string expirationDate;
	std::string cvv;
	unsigned int password;
	bool active; // 是否激活
	std::vector<std::string> transactionHistory; // 交易记录
protected:
	// 添加交易记录
	void addTransaction(const std::string& transaction) {
		// 获取当前时间作为交易时间
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
		addTransaction("开户成功");
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
	// 验证密码
	bool verifyPassword(unsigned int inputPassword) const {
		return password == inputPassword;
	}
	// 修改密码
	bool changePassword(unsigned int oldPassword, unsigned int newPassword) {
		if (verifyPassword(oldPassword)) {
			password = newPassword;
			addTransaction("密码修改成功");
			return true;
		}
		return false;
	}
	// 注销账户
	bool closeAccount(unsigned int inputPassword) {
		if (verifyPassword(inputPassword)) {
			active = false;
			addTransaction("账户注销");
			return true;
		}
		return false;
	}
	// 账户是否激活
	bool isActive() const {
		return active;
	}
	// 重新激活账户
	bool reactivateAccount(unsigned int inputPassword) {
		if (verifyPassword(inputPassword)) {
			active = true;
			addTransaction("账户重新激活");
			return true;
		}
		return false;
	}
	// 显示交易历史
	void showTransactionHistory() const {
		std::cout << "\n交易历史记录:" << std::endl;
		if (transactionHistory.empty()) {
			std::cout << "暂无交易记录" << std::endl;
			return;
		}

		for (const auto& transaction : transactionHistory) {
			std::cout << transaction << std::endl;
		}
	}
    // 生成唯一16位的银行卡号(使用时间、线程的哈希实现)
	unsigned long long generateCardNumber() {
		static int counter = 0;
		// 使用当前时间作为种子
		auto now = std::chrono::high_resolution_clock::now();
		auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

		// 组合多个元素以确保唯一性
		std::string uniqueStr =
			std::to_string(timestamp) +
			std::to_string(counter++) +
			std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

		// 生成16位卡号
		// 第1位: 银行卡类型，信用卡以4开头，借记卡以6开头
		std::string cardNumber = "4"; // 默认为信用卡前缀

		// 后续14位从哈希值中提取
		std::size_t hashValue = std::hash<std::string>{}(uniqueStr);
		std::string hashStr = std::to_string(hashValue);

		// 确保至少有14位数字可用
		while (hashStr.length() < 14) {
			hashStr += std::to_string(counter++);
		}

		// 取前14位
		cardNumber += hashStr.substr(0, 14);

		// 计算并添加校验位（第16位）
		int sum = 0;
		for (int i = 0; i < 15; i++) {
			int digit = cardNumber[i] - '0';
			if (i % 2 == 0) { // 偶数位（从0开始计数）
				digit *= 2;
				if (digit > 9) {
					digit -= 9;
				}
			}
			sum += digit;
		}
		int checkDigit = (10 - (sum % 10)) % 10;
		cardNumber += std::to_string(checkDigit);

		// 转换为数字返回
		unsigned long long result = std::stoull(cardNumber);
		return result;
	}
	// 生成唯一的CVV码
	std::string generateCVV() {
		static int counter = 0;
		// 使用当前时间作为种子
		auto now = std::chrono::high_resolution_clock::now();
		auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		// 组合多个元素以确保唯一性
		std::string uniqueStr =
			std::to_string(timestamp) +
			std::to_string(counter++) +
			std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));
		// 使用标准哈希函数生成唯一ID
		return std::to_string(std::hash<std::string>{}(uniqueStr) % 1000); // CVV为3位数
	}
	//过期时间 - 全面解决方案
	std::string generateExpirationDate() {
		// 获取当前时间
		std::tm timeinfo;
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_s(&timeinfo, &now);

		// 保存原始日期信息
		int original_day = timeinfo.tm_mday;
		int original_month = timeinfo.tm_mon;
		int original_year = timeinfo.tm_year;

		// 增加5年
		timeinfo.tm_year += 5;

		// 检查月份和日期的有效性
		int year = timeinfo.tm_year + 1900;

		// 根据月份确定最大天数
		int max_days = 31; // 默认为31天

		if (timeinfo.tm_mon == 1) { // 二月
			// 检查闰年
			bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
			max_days = isLeapYear ? 29 : 28;
		}
		else if (timeinfo.tm_mon == 3 || timeinfo.tm_mon == 5 ||
			timeinfo.tm_mon == 8 || timeinfo.tm_mon == 10) {
			// 4, 6, 9, 11月
			max_days = 30;
		}

		// 调整超出最大天数的情况
		if (timeinfo.tm_mday > max_days) {
			timeinfo.tm_mday = max_days;
		}

		// 标准化日期
		std::time_t expTime = std::mktime(&timeinfo);
		if (expTime == -1) {
			// 如果发生错误，使用简单但可靠的方法
			// 先重置为当前时间
			localtime_s(&timeinfo, &now);

			// 先加4年（避开闰年问题）
			timeinfo.tm_year += 4;
			std::mktime(&timeinfo); // 标准化

			// 再加1年
			timeinfo.tm_year += 1;
			std::mktime(&timeinfo); // 标准化
		}

		// 格式化日期
		char buffer[11];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
		return std::string(buffer);
	}

	virtual void input() {
		std::cout << "请输入持卡人姓名: ";
		std::getline(std::cin, owner.name);
		std::cout << "请输入持卡人地址: ";
		std::getline(std::cin, owner.address);
		std::cout << "请输入持卡人电话: ";
		std::getline(std::cin, owner.phoneNumber);
		std::cout << "请输入持卡人年龄: ";
		std::cin >> owner.age;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// 设置密码
		unsigned int newPassword, confirmPassword;
		do {
			std::cout << "请设置6位数字密码: ";
			std::cin >> newPassword;
			std::cout << "请再次输入密码: ";
			std::cin >> confirmPassword;

			if (newPassword != confirmPassword) {
				std::cout << "两次输入的密码不一致，请重新输入！" << std::endl;
			}
		} while (newPassword != confirmPassword);

		password = newPassword;

		cardNumber = generateCardNumber();
		expirationDate = generateExpirationDate();
		cvv = generateCVV();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	virtual void display() const {
		std::cout << "银行卡信息:" << std::endl;
		std::cout << "持卡人姓名: " << owner.name << std::endl;
		std::cout << "持卡人地址: " << owner.address << std::endl;
		std::cout << "持卡人电话: " << owner.phoneNumber << std::endl;
		std::cout << "持卡人年龄: " << owner.age << std::endl;
		std::cout << "银行卡号: " << cardNumber << std::endl;
		std::cout << "过期时间: " << expirationDate << std::endl;
		std::cout << "CVV码: " << cvv << std::endl;
		std::cout << "卡片状态: " << (active ? "激活" : "已注销") << std::endl;
	}
};

// 信用卡类
class CreditCard : public BankCard {
private:
	double creditLimit;
	double currentBalance;
	double outstandingBalance; // 未还款金额

public:
	CreditCard(std::string expDate, std::string cvvCode, Person owner, unsigned int pass, double limit)
		: BankCard(expDate, cvvCode, owner, pass), creditLimit(limit), currentBalance(0.0), outstandingBalance(0.0) {
	}
	// 消费
	bool charge(double amount) {
		if (!isActive()) {
			std::cout << "账户已注销，无法消费" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "消费金额必须为正数" << std::endl;
			return false;
		}

		if (currentBalance + amount > creditLimit) {
			std::cout << "超出信用额度，消费失败" << std::endl;
			return false;
		}

		currentBalance += amount;
		outstandingBalance += amount;
		std::string transaction = "消费 $" + std::to_string(amount) + "，当前已用额度: $" + std::to_string(currentBalance);
		addTransaction(transaction);
		return true;
	}

	// 还款
	bool makePayment(double amount) {
		if (!isActive()) {
			std::cout << "账户已注销，无法还款" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "还款金额必须为正数" << std::endl;
			return false;
		}

		if (amount > outstandingBalance) {
			amount = outstandingBalance; // 如果还款超过欠款，只还欠款金额
		}

		outstandingBalance -= amount;
		currentBalance -= amount;

		std::string transaction = "还款 $" + std::to_string(amount) + "，剩余欠款: $" + std::to_string(outstandingBalance);
		addTransaction(transaction);
		return true;
	}

	// 获取可用额度
	double getAvailableCredit() const {
		return creditLimit - currentBalance;
	}

	// 获取已使用额度
	double getCurrentBalance() const {
		return currentBalance;
	}

	// 获取未还款金额
	double getOutstandingBalance() const {
		return outstandingBalance;
	}

	void input() override {
		BankCard::input();
		std::cout << "请输入信用额度: $";
		std::cin >> creditLimit;
	}

	// 显示信用卡信息
	void display() const override {
		std::cout << "信用卡信息:" << std::endl;
		BankCard::display();
		std::cout << "信用额度: $" << creditLimit << std::endl;
		std::cout << "已用额度: $" << currentBalance << std::endl;
		std::cout << "剩余可用额度: $" << getAvailableCredit() << std::endl;
		std::cout << "未还款金额: $" << outstandingBalance << std::endl;
	}
};	

// 借记卡类
class DebitCard : public BankCard {
private:
	double balance;
public:
	DebitCard(std::string expDate, std::string cvvCode, Person owner, unsigned int pass)
		: BankCard(expDate, cvvCode, owner, pass), balance(0.0) {
	}
	// 存款
	bool deposit(double amount) {
		if (!isActive()) {
			std::cout << "账户已注销，无法存款" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "存款金额必须为正数" << std::endl;
			return false;
		}

		balance += amount;
		std::string transaction = "存款 $" + std::to_string(amount) + " 成功，当前余额: $" + std::to_string(balance);
		addTransaction(transaction);
		return true;
	}
	// 取款
	bool withdraw(double amount, unsigned int inputPassword) {
		if (!isActive()) {
			std::cout << "账户已注销，无法取款" << std::endl;
			return false;
		}

		if (!verifyPassword(inputPassword)) {
			std::cout << "密码错误，取款失败" << std::endl;
			return false;
		}

		if (amount <= 0) {
			std::cout << "取款金额必须为正数" << std::endl;
			return false;
		}

		if (amount > balance) {
			std::cout << "余额不足，取款失败" << std::endl;
			return false;
		}

		balance -= amount;
		std::string transaction = "取款 $" + std::to_string(amount) + " 成功，当前余额: $" + std::to_string(balance);
		addTransaction(transaction);
		return true;
	}
	// 查询余额
	double getBalance() const {
		return balance;
	}

	void input() override {
		BankCard::input();
		double initialDeposit = 0.0;
		std::cout << "请输入初始存款金额: $";
		std::cin >> initialDeposit;
		if (initialDeposit > 0) {
			balance = initialDeposit;
			std::string transaction = "初始存款 $" + std::to_string(initialDeposit);
			addTransaction(transaction);
		}
	}

	// 显示借记卡信息
	void display() const override {
		std::cout << "借记卡信息:" << std::endl;
		BankCard::display();
		std::cout << "当前余额: $" << balance << std::endl;
	}
};

// 银行卡管理类
class CardManager {
private:
	std::vector<BankCard*> allCards;
	std::vector<CreditCard*> creditCards;
	std::vector<DebitCard*> debitCards;

public:
	// 添加银行卡
	void addCard(BankCard* card) {
		allCards.push_back(card);

		// 检查卡片类型并添加到相应的容器
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

	// 获取统计信息
	unsigned int getTotalCardCount() const {
		return allCards.size();
	}
	unsigned int getCreditCardCount() const {
		return creditCards.size();
	}
	unsigned int getDebitCardCount() const {
		return debitCards.size();
	}

	// 根据卡号查找卡片
	BankCard* findCardByNumber(unsigned long long cardNumber) {
		for (auto card : allCards) {
			if (card->getCardNumber() == cardNumber) {
				return card;
			}
		}
		return nullptr;
	}

	// 显示所有卡片信息
	void displayAllCards() const {
		std::cout << "银行卡总数: " << getTotalCardCount() << std::endl;
		std::cout << "信用卡数量: " << getCreditCardCount() << std::endl;
		std::cout << "借记卡数量: " << getDebitCardCount() << std::endl;

		std::cout << "\n所有卡片信息:" << std::endl;
		for (size_t i = 0; i < allCards.size(); i++) {
			std::cout << "卡片 #" << (i + 1) << ":" << std::endl;
			if (dynamic_cast<CreditCard*>(allCards[i])) {
				dynamic_cast<CreditCard*>(allCards[i])->display();
			}
			else if (dynamic_cast<DebitCard*>(allCards[i])) {
				dynamic_cast<DebitCard*>(allCards[i])->display();
			}
			std::cout << "---------------------------" << std::endl;
		}
	}
	// 添加银行卡手动输入
	void addCardManually() {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区

		Person newOwner;
		std::cout << "\n==== 添加新银行卡 ====" << std::endl;

		// 选择卡片类型
		int cardType;
		std::cout << "请选择银行卡类型：" << std::endl;
		std::cout << "1. 信用卡" << std::endl;
		std::cout << "2. 借记卡" << std::endl;
		std::cout << "请输入选择 (1-2): ";
		std::cin >> cardType;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区

		if (cardType != 1 && cardType != 2) {
			std::cout << "无效的选择，默认创建借记卡。" << std::endl;
			cardType = 2;
		}

		BankCard* newCard = nullptr;

		if (cardType == 1) {
			// 创建信用卡
			CreditCard* creditCard = new CreditCard("", "", newOwner, 123456, 0.0);
			creditCard->input();
			newCard = creditCard;
			std::cout << "\n信用卡创建成功！" << std::endl;
		}
		else {
			// 创建借记卡
			DebitCard* debitCard = new DebitCard("", "", newOwner,123456);
			debitCard->input();
			newCard = debitCard;
			std::cout << "\n借记卡创建成功！" << std::endl;
		}

		// 添加到管理器
		addCard(newCard);
	}
	// 在 CardManager 类中添加搜索功能
	void searchCard() {
		int searchType;
		std::cout << "\n==== 查找银行卡 ====" << std::endl;
		std::cout << "1. 按卡号查找" << std::endl;
		std::cout << "2. 按持卡人姓名查找" << std::endl;
		std::cout << "请输入查找方式 (1-2): ";
		std::cin >> searchType;

		if (searchType == 1) {
			unsigned long long cardNumber;
			std::cout << "请输入卡号: ";
			std::cin >> cardNumber;

			BankCard* card = findCardByNumber(cardNumber);
			if (card) {
				std::cout << "\n找到匹配的卡片：" << std::endl;
				if (dynamic_cast<CreditCard*>(card)) {
					dynamic_cast<CreditCard*>(card)->display();
				}
				else if (dynamic_cast<DebitCard*>(card)) {
					dynamic_cast<DebitCard*>(card)->display();
				}
			}
			else {
				std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			}
		}
		else if (searchType == 2) {
			std::string name;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除输入缓冲区
			std::cout << "请输入持卡人姓名: ";
			std::getline(std::cin, name);

			bool found = false;
			for (auto card : allCards) {
				if (card->getOwnerName() == name) {
					found = true;
					std::cout << "\n找到匹配的卡片：" << std::endl;
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
				std::cout << "未找到持卡人为 " << name << " 的银行卡。" << std::endl;
			}
		}
		else {
			std::cout << "无效的选择。" << std::endl;
		}
	}
	// 存款操作
	void depositToAccount() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== 存款操作 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		DebitCard* debitCard = dynamic_cast<DebitCard*>(card);
		if (!debitCard) {
			std::cout << "该卡不是借记卡，无法存款。" << std::endl;
			return;
		}

		std::cout << "请输入存款金额: $";
		std::cin >> amount;

		if (debitCard->deposit(amount)) {
			std::cout << "存款成功! 当前余额: $" << debitCard->getBalance() << std::endl;
		}
		else {
			std::cout << "存款失败!" << std::endl;
		}
	}

	// 取款操作
	void withdrawFromAccount() {
		unsigned long long cardNumber;
		double amount;
		unsigned int password;

		std::cout << "\n==== 取款操作 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		DebitCard* debitCard = dynamic_cast<DebitCard*>(card);
		if (!debitCard) {
			std::cout << "该卡不是借记卡，无法取款。" << std::endl;
			return;
		}

		std::cout << "请输入取款金额: $";
		std::cin >> amount;
		std::cout << "请输入密码: ";
		std::cin >> password;

		if (debitCard->withdraw(amount, password)) {
			std::cout << "取款成功! 当前余额: $" << debitCard->getBalance() << std::endl;
		}
		else {
			std::cout << "取款失败!" << std::endl;
		}
	}

	// 信用卡消费
	void chargeToCreditCard() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== 信用卡消费 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		CreditCard* creditCard = dynamic_cast<CreditCard*>(card);
		if (!creditCard) {
			std::cout << "该卡不是信用卡，无法进行消费。" << std::endl;
			return;
		}

		std::cout << "请输入消费金额: $";
		std::cin >> amount;

		if (creditCard->charge(amount)) {
			std::cout << "消费成功! 剩余可用额度: $" << creditCard->getAvailableCredit() << std::endl;
		}
		else {
			std::cout << "消费失败!" << std::endl;
		}
	}

	// 信用卡还款
	void makePaymentToCreditCard() {
		unsigned long long cardNumber;
		double amount;

		std::cout << "\n==== 信用卡还款 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		CreditCard* creditCard = dynamic_cast<CreditCard*>(card);
		if (!creditCard) {
			std::cout << "该卡不是信用卡，无法进行还款。" << std::endl;
			return;
		}

		std::cout << "当前未还款金额: $" << creditCard->getOutstandingBalance() << std::endl;
		std::cout << "请输入还款金额: $";
		std::cin >> amount;

		if (creditCard->makePayment(amount)) {
			std::cout << "还款成功! 剩余未还款金额: $" << creditCard->getOutstandingBalance() << std::endl;
		}
		else {
			std::cout << "还款失败!" << std::endl;
		}
	}

	// 修改密码
	void changeCardPassword() {
		unsigned long long cardNumber;
		unsigned int oldPassword, newPassword, confirmPassword;

		std::cout << "\n==== 修改密码 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		std::cout << "请输入原密码: ";
		std::cin >> oldPassword;

		do {
			std::cout << "请输入新密码: ";
			std::cin >> newPassword;
			std::cout << "请确认新密码: ";
			std::cin >> confirmPassword;

			if (newPassword != confirmPassword) {
				std::cout << "两次输入的密码不一致，请重新输入！" << std::endl;
			}
		} while (newPassword != confirmPassword);

		if (card->changePassword(oldPassword, newPassword)) {
			std::cout << "密码修改成功!" << std::endl;
		}
		else {
			std::cout << "密码修改失败，原密码错误!" << std::endl;
		}
	}

	// 注销账户
	void closeAccount() {
		unsigned long long cardNumber;
		unsigned int password;

		std::cout << "\n==== 注销账户 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		std::cout << "请输入密码: ";
		std::cin >> password;

		if (card->closeAccount(password)) {
			std::cout << "账户注销成功!" << std::endl;
		}
		else {
			std::cout << "账户注销失败，密码错误!" << std::endl;
		}
	}

	// 重新激活账户
	void reactivateAccount() {
		unsigned long long cardNumber;
		unsigned int password;

		std::cout << "\n==== 重新激活账户 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		if (card->isActive()) {
			std::cout << "该账户已经是激活状态，无需重新激活。" << std::endl;
			return;
		}

		std::cout << "请输入密码: ";
		std::cin >> password;

		if (card->reactivateAccount(password)) {
			std::cout << "账户重新激活成功!" << std::endl;
		}
		else {
			std::cout << "账户重新激活失败，密码错误!" << std::endl;
		}
	}

	// 查看交易历史
	void viewTransactionHistory() {
		unsigned long long cardNumber;

		std::cout << "\n==== 查看交易历史 ====" << std::endl;
		std::cout << "请输入卡号: ";
		std::cin >> cardNumber;

		BankCard* card = findCardByNumber(cardNumber);
		if (!card) {
			std::cout << "未找到卡号为 " << cardNumber << " 的银行卡。" << std::endl;
			return;
		}

		card->showTransactionHistory();
	}
	// 析构函数中释放内存
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
	// 创建卡片管理器
	CardManager cardManager;
	int choice = -1;

	while (choice != 0) {
		mainMenu();
		std::cout << "请输入选择 (0-11): ";
		std::cin >> choice;

		switch (choice) {
		case 1: // 开户管理
			cardManager.addCardManually();
			break;
		case 2: // 存款管理
			cardManager.depositToAccount();
			break;
		case 3: // 取款管理
			cardManager.withdrawFromAccount();
			break;
		case 4: // 信用卡消费
			cardManager.chargeToCreditCard();
			break;
		case 5: // 信用卡还款
			cardManager.makePaymentToCreditCard();
			break;
		case 6: // 查询账户
			cardManager.searchCard();
			break;
		case 7: // 修改密码
			cardManager.changeCardPassword();
			break;
		case 8: // 查看交易历史
			cardManager.viewTransactionHistory();
			break;
		case 9: // 账户注销
			cardManager.closeAccount();
			break;
		case 10: // 重新激活账户
			cardManager.reactivateAccount();
			break;
		case 11: // 显示所有账户
			cardManager.displayAllCards();
			break;
		case 0:
			std::cout << "感谢使用银行账户管理系统，再见！" << std::endl;
			break;
		default:
			std::cout << "无效的选择，请重试。" << std::endl;
			break;
		}

		// 在每次操作后暂停
		if (choice != 0) {
			std::cout << "\n按 Enter 键继续...";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
			system("cls"); // 清屏，Windows系统
		}
	}

	return 0;
}