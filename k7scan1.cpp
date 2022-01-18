#include "k7scan1.h"
#include "Dijkstra.h"

extern string location[ANZAHL];
extern string cities[ANZAHL];

/*
 *	Lexical analyzer states.
 */
enum lexstate {
	L_START, L_INT, L_IDENT, L_STRING, L_STRING2,
	L_COMMENT, L_TEXT_COMMENT, L_LINE_COMMENT, L_END_TEXT_COMMENT
};

const int STRING1 = 3;
const int IDENTIFIER = 4;
const int INTEGER1 = 5;
const int TOKENSTART = 300;
const int DEPARTURE = 306;
const int ARRIVAL = 307;

class CParser
{
public:

	string yytext;								//input buffer
	struct tyylval {								//value return
		string s;								//structure
		int i;
	}yylval;
	FILE *IP_Input;								//Input File
	FILE *IP_Error;								//Error Output
	FILE *IP_List;								//List Output
	int  IP_LineNumber;							//Line counter
	int ugetflag;								//checks ungets
	int prflag;									//controls printing
	map<string, int> IP_Token_table;				//Tokendefinitions
	map<int, string> IP_revToken_table;			//reverse Tokendefinitions


	int yylex();						//lexial analyser
	void yyerror(char *ers);			//error reporter
	int IP_MatchToken(string &tok);		//checks the token
	void InitParse(FILE *inp, FILE *err, FILE *lst);
	int	yyparse();						//parser
	void pr_tokentable();				//test output for tokens
	void IP_init_token_table();			//loads the tokens
	void Load_tokenentry(string str, int index);//load one token
	void PushString(char c);			//Used for dtring assembly
	CParser() { IP_LineNumber = 1; ugetflag = 0; prflag = 0; };	//Constructor
};
//------------------------------------------------------------------------

// Adds a character to the string value
void CParser::PushString(char c)
{
	yylval.s += c;
}
//------------------------------------------------------------------------
void CParser::Load_tokenentry(string str, int index)
{
	IP_Token_table[str] = index;
	IP_revToken_table[index] = str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("STRING1", 3);
	Load_tokenentry("IDENTIFIER", 4);
	Load_tokenentry("INTEGER1", 5);


	int ii = TOKENSTART;
	Load_tokenentry("AND", ii++);
	Load_tokenentry("OR", ii++);
	Load_tokenentry("Begin", ii++);
	Load_tokenentry("End", ii++);
	Load_tokenentry("Departure", 306);
	Load_tokenentry("Arrival", 307);

}
//------------------------------------------------------------------------

void CParser::pr_tokentable()
{

	typedef map<string, int>::const_iterator CI;
	const char* buf;

	printf("Symbol Table ---------------------------------------------\n");

	for (CI p = IP_Token_table.begin(); p != IP_Token_table.end(); ++p) {
		buf = p->first.c_str();
		printf(" key:%s", buf);
		printf(" val:%d\n", p->second);;
	}
}
//------------------------------------------------------------------------

int	CParser::yyparse()
{
	bool lctn1 = false, lctn2 = false;
	int tok;
	//if (prflag)fprintf(IP_List, "%5d ", (int)IP_LineNumber);
	/*
	*	Go parse things!
	*/
	while ((tok = yylex()) != 0) {
		if (tok == DEPARTURE) {
			lctn1 = true;
			//cout << "dep" << endl;
		}
		if (tok == ARRIVAL) {
			lctn2 = true;
			//cout << "arr" << endl;
		}
		if (tok == IDENTIFIER) {
			if (lctn1 == true) {
				location[0] = yylval.s.c_str();
				//std::transform(location[0].begin(), location[0].end(), location[0].begin(), std::tolower);
				lctn1 = false;
			}
			if (lctn2 == true) {
				location[1] = yylval.s.c_str();
				lctn2 = false;
			}
		}

		//printf("%d ", tok);
		//if (tok == STRING1)
		//	printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
		//else
		//	if (tok == INTEGER1)
		//		printf("%s %d ", IP_revToken_table[tok].c_str(), yylval.i);
		//	else
		//		if (tok == IDENTIFIER)
		//			printf("%s %s ", IP_revToken_table[tok].c_str(), yylval.s.c_str());
		//		else
		//			if (tok == STUDENT) {
		//				//cout << IP_revToken_table[tok].c_str() << yylval.s.c_str();
		//				cout << yylval.s.c_str();
		//				cout << " ---Student found!---";	}
		//			else
		//				if (tok >= TOKENSTART)
		//					printf("%s ", IP_revToken_table[tok].c_str());
		//				else
		//					printf("%c ", tok);
		//if (!prflag)printf("\n");
	}
	return 0;

}
//------------------------------------------------------------------------

/*
 *	Parse Initfile:
 *
 *	  This builds the context tree and then calls the real parser.
 *	It is passed two file streams, the first is where the input comes
 *	from; the second is where error messages get printed.
 */
void CParser::InitParse(FILE *inp, FILE *err, FILE *lst)

{

	/*
	*	Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List = lst;

	IP_LineNumber = 1;
	ugetflag = 0;
	/*
	*	Define both the enabled token and keyword strings.
	*/
	IP_init_token_table();
}
//------------------------------------------------------------------------

/*
 *	yyerror:
 *
 *	  Standard error reporter, it prints out the passed string
 *	preceeded by the current filename and line number.
 */
void CParser::yyerror(char *ers)

{
	fprintf(IP_Error, "line %d: %s\n", IP_LineNumber, ers);
}
//------------------------------------------------------------------------

int CParser::IP_MatchToken(string &tok)
{
	int retval;
	if (IP_Token_table.find(tok) != IP_Token_table.end()) {
		retval = (IP_Token_table[tok]);
	}
	else {
		retval = (0);
	}
	return retval;
}

//------------------------------------------------------------------------

/*
 *	yylex:
 *
 */
int CParser::yylex()
{
	//Locals
	int c;
	lexstate s;
	/*
	*	Keep on sucking up characters until we find something which
	*	explicitly forces us out of this function.
	*/
	for (s = L_START, yytext = ""; 1;) {
		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		if (!ugetflag) {
			if (c != EOF)if (prflag)fprintf(IP_List, "%c", c);
		}
		else ugetflag = 0;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START:
			if (isdigit(c)) {
				s = L_INT;
			}
			else if (isalpha(c) || c == '\\') {
				s = L_IDENT;
			}
			else if (isspace(c)) {
				if (c == '\n') {
					IP_LineNumber += 1;
					if (prflag)
						fprintf(IP_List, "%5d ", (int)IP_LineNumber);
				}
				yytext = "";
			}
			else if (c == '/') {
				yytext = "";
				s = L_COMMENT;
			}
			else if (c == '"') {
				s = L_STRING;
				yylval.s = "";
			}
			else if (c == EOF) {
				return ('\0');
			}
			else {
				return (c);
			}
			break;

		case L_COMMENT:
			if (c == '/')
				s = L_LINE_COMMENT;
			else	if (c == '*')
				s = L_TEXT_COMMENT;
			else {
				Ungetc(c);
				return('/');	/* its the division operator not a comment */
			}
			break;
		case L_LINE_COMMENT:
			if (c == '\n') {
				s = L_START;
				Ungetc(c);
			}
			yytext = "";
			break;
		case L_TEXT_COMMENT:
			if (c == '\n') {
				IP_LineNumber += 1;
			}
			else if (c == '*')
				s = L_END_TEXT_COMMENT;
			yytext = "";
			break;
		case L_END_TEXT_COMMENT:
			if (c == '/') {
				s = L_START;
			}
			else {
				s = L_TEXT_COMMENT;
			}
			yytext = "";
			break;

			/*
			 *	Suck up the integer digits.
			 */
		case L_INT:
			if (isdigit(c)) {
				break;
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.i = atoi(yylval.s.c_str());
				return (INTEGER1);
			}
			break;

			/*
			 *	Grab an identifier, see if the current context enables
			 *	it with a specific token value.
			 */

		case L_IDENT:
			if (isalpha(c) || isdigit(c) || c == '_')
				break;
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1);
			yylval.s = yytext;
			if (c = IP_MatchToken(yytext)) {
				return (c);
			}
			else {
				return (IDENTIFIER);
			}

			/*
			*	Suck up string characters but once resolved they should
			*	be deposited in the string bucket because they can be
			*	arbitrarily long.
			*/
		case L_STRING2:
			s = L_STRING;
			if (c == '"') {// >\"< found
				PushString((char)c);
			}
			else {
				if (c == '\\') {// >\\< found
					PushString((char)c);
				}
				else {
					PushString((char)'\\');// >\x< found
					PushString((char)c);
				}
			}
			break;
		case L_STRING:
			if (c == '\n')
				IP_LineNumber += 1;
			else if (c == '\r');
			else if (c == '"' || c == EOF) {
				return (STRING1);
			}
			else if (c == '\\') {
				s = L_STRING2;
				//PushString((char)c);
			}
			else
				PushString((char)c);
			break;
		default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}
//------------------------------------------------------------------------

int comp_string_location(string str_location) {
	for (auto ii = 0; ii < ANZAHL; ii++) {
		if (cities[ii] == str_location) return ii;
	}
	
	return ANZAHL+1;
}



int read_txt_file() {
	FILE *inf = NULL;
	//char fistr[100];
	fopen_s(&inf, "Input.txt", "r");
	if (inf == NULL) {
		cout << "Cannot open input file Input.txt" << endl;
		return EXIT_FAILURE;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	obj.yyparse();

	return EXIT_SUCCESS;
}
