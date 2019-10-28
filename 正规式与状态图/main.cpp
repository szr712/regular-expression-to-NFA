#include<string>
#include<iostream>
#include<fstream>
#include<stack>
#include<map>
#include<vector>
#include<set>
using namespace std;

struct edge
{
	char weight;
	char next;
};

// edge��NFAͼ�еıߣ�
// option������ķ��ż���
// expression�����������ʽ��
// infix���������ӷ�������ʽ��
// suffix��ת���ɵĺ�׺���ʽ��
// postion��״̬����
// precedence���涨��������ȼ���
// nfa�����ɵ�NFA��ʹ��map������һ��char��edge������ӳ�䣬char����NFA�еĶ��㣬Ҳ����������һ��������Ӷ�������ߵļ��ϵ�ӳ�䣩��
// tfunction��ת�ƺ�����
string option,expression, suffix, infix, postion;
map<char, int> precedence;
char start='@';
map<char, vector<edge>> nfa;
vector<string> tfunction;


//������������ȼ�
void setprecedence() {
	precedence['*'] = 3;
	precedence['.'] = 2;
	precedence['|'] = 1;
}

//���ļ��ж�ȡ����ʽ�������ж��Ƿ��зǷ�����
void input() {
	ifstream in("input.txt");
	if (!in) {
		cout << "���ļ����������˳���" << endl;
		system("Pause");
		exit(1);
	}
	in >> option >> expression;
	for (int i = 0; i < expression.length(); i++) {
		if (option.find(expression[i])!=option.npos|| expression[i]=='|' || expression[i]=='(' || expression[i] == ')' || expression[i] == '*' || expression[i] == '.') {
			//�������ַ�
		}
		else {
			cout << "�Ƿ����룡" << endl;
			system("Pause");
			exit(1);
		}
	}
}

//�������ӷ�
void toinfix() {
	for (int i = 0; i < expression.length(); i++) {
		char tmp = expression[i];
		char next;
		if (i == expression.length() - 1) {
			next = '\0';
		}
		else {
			next = expression[i + 1];
		}
		if (((tmp != '('&&tmp != '.'&&tmp != '|') || tmp == ')' || tmp == '*') && (next != ')'&&next != '*'&&next != '|'&&next != '.'&&next != '\0')) {
			infix = infix + tmp + '.';
		}
		else {
			infix = infix + tmp;
		}
	}
}

//��׺ת��׺
void tosuffix() {
	stack<char> cstack;
	for (int i = 0; i < infix.length(); i++) {
		char tmp = infix[i];
		if (tmp == '(') cstack.push(tmp);
		else if (tmp == ')') {
			while (cstack.top() != '(') {
				char t = cstack.top();
				cstack.pop();
				suffix = suffix + t;
			}
			cstack.pop();
		}
		else if(tmp == '|'|| tmp == '*' || tmp == '.')
		{
			while (!cstack.empty()) {
				char t = cstack.top();
				if (precedence[t] >= precedence[tmp]) {
					suffix = suffix + t;
					cstack.pop();
				}

				else if (precedence[t] < precedence[tmp]) {
					cstack.push(tmp);
					break;
				}
			}
			if (cstack.empty())cstack.push(tmp);
		}
		else {
			suffix = suffix + tmp;
		}
	}
	while (!cstack.empty()) {
		char t = cstack.top();
		cstack.pop();
		suffix = suffix + t;
	}
}

//�ҵ���ǰnfa����̬
char findback(char a,set<char> x) {
	if (nfa[a].empty()) return a;
	else {
		x.insert(a);
		int i = 0;
		while (x.find(nfa[a][i].next) != x.end()) {
			i++;
		}
		char t=findback(nfa[a][i].next,x);
		return t;
	}
}

//����׺���ʽת����nfa
char tonfa() {
	stack<char>h;
	set<char>x;
	for (int i = 0; i < suffix.length(); i++) {
		char tmp = suffix[i];
		if (tmp == '.') {
			edge e;
			e.weight = '~';
			char t = h.top();
			h.pop();
			x.clear();
			char b = findback(h.top(),x);
			e.next = t;
			nfa[b].push_back(e);
		}
		else if (tmp == '*') {
			edge e;
			e.weight = '~';
			start++;
			char t = h.top();
			h.pop();
			x.clear();
			char b = findback(t,x);
			e.next = t;
			nfa[start].push_back(e);
			nfa[b].push_back(e);
			e.next = start + 1;
			nfa[b].push_back(e);
			nfa[start].push_back(e);
			h.push(start);
			postion = postion + start + char(start + 1);
			start++;
		}
		else if (tmp == '|') {
			edge e;
			e.weight = '~';
			start++;
			char t = h.top();
			h.pop();
			x.clear();
			char b = findback(t,x);
			e.next = t;
			nfa[start].push_back(e);
			e.next = start+1;
			nfa[b].push_back(e);
			t = h.top();
			h.pop();
			x.clear();
			b = findback(t,x);
			e.next = t;
			nfa[start].push_back(e);
			e.next = start + 1;
			nfa[b].push_back(e);
			h.push(start);
			postion = postion + start + char(start + 1);
			start++;
		}
		else {
			start++;
			edge e;
			e.weight = tmp;
			e.next = start + 1;
			nfa[start].push_back(e);
			h.push(start);
			postion = postion + start + char(start+1);
			start++;
		}
	}
	return h.top();
}

//���ת������
void getfunction() {
	for (auto inter = nfa.begin(); inter != nfa.end(); inter++) {
		for (int i = 0; i < inter->second.size(); i++) {
			string tmp;
			tmp =tmp+ '(' + inter->first + ',' + inter->second[i].weight + ',' + inter->second[i].next + ')';
			tfunction.push_back(tmp);
		}
	}
}


int main() {
	setprecedence();
	input();
	toinfix();
	tosuffix();
	char s=tonfa(); 
	ofstream out("output.txt");
	printf("����ʽΪ��");
	cout << expression << endl;
	printf("�������ӵ��");
	cout << infix << endl;
	printf("תΪ��׺���ʽ��");
	cout << suffix << endl;
	printf("\n\n���Ϊ������~��ʾ�գ�\n״̬��Ϊ��");
	out << postion << endl;
	cout << postion << endl;
	printf("���ż�Ϊ��");
	out << option << endl;
	cout << option << endl;
	printf("��̬��Ϊ��");
	printf("%c\n", s);
	out << s << endl;
	printf("��̬��Ϊ��");
	set<char>x;
	cout << findback(s,x) << endl;
	out << findback(s, x) << endl;
	printf("ת�ƺ���Ϊ��\n");
	getfunction();
	for (int i = 0; i < tfunction.size(); i++) {
		cout << tfunction[i] << endl;
		out << tfunction[i] << endl;
	}
	system("Pause");
}