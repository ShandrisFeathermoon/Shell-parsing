#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

typedef struct tagHouseInfo {
	std::string title;
	std::string url;
	std::string id;
	std::string info;
	std::string follow;
	std::string taxfree;
	std::string totalprice;
	std::string unitprice;
	void Reset()
	{
		title.clear();
		url.clear();
		id.clear();
		info.clear();
		follow.clear();
		taxfree.clear();
		totalprice.clear();
		unitprice.clear();
	}
}HouseInfo;

void GetInfo(const char *key, std::string *str, char *buf);
void GetDivInfo(const char *key, std::string *str, char *buf);
void GetDivInfo(std::string *str, char *buf);
bool GetTitleInfo(FILE *file, HouseInfo *hi);
bool GetHouseInfo(FILE *file, HouseInfo *hi);
bool GetFollowInfo(FILE *file, HouseInfo *hi);
bool GetTagInfo(FILE *file, HouseInfo *hi);
bool GetTotalPrice(FILE *file, HouseInfo *hi);
bool GetUnitPrice(FILE *file, HouseInfo *hi);

void WriteCsv(FILE *file, const HouseInfo *hi);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need at least one parameter!\n");
		return 0;
	}

	bool append = false;
	char outfile[256] = {0};
	if (argc == 2) {
		strcpy(outfile, argv[1]);
		char *ptr = strrchr(outfile, '.');
		if (ptr) {
			*ptr = 0;
			strcat(outfile, ".csv");
		}
	}
	else {
		strcpy(outfile, argv[2]);
		if (argc == 4) {
			append = true;
		}
	}

	FILE *file = fopen(argv[1], "rb");
	if (file) {
		FILE *out = fopen(outfile, append ? "ab+" : "wb");
		if (!out) {
			printf("Failed to create output file \n");
			fclose(file);
			return 1;
		}
		char buf[4097] = {0};
		memset(buf, 0, 4097);
		bool start = false;
		bool divbegin = false;
		int count = 0;
		HouseInfo hi;
		while (!feof(file)) {
			fgets(buf, 4096, file);
			if (strstr(buf, "ul class=\"sellListContent\"")) {
				start = true;
			}
			if (start) {
				if (strstr(buf, "</ul>")) {
					break;
				}
				else if (strstr(buf, "li class=\"clear\"")) {
					divbegin = true;
					count++;
				}
				else if (strstr(buf, "</li>")) {
					hi.Reset();
					continue;
				}
				if (divbegin) {
					GetTitleInfo(file, &hi);
					GetHouseInfo(file, &hi);
					GetFollowInfo(file, &hi);
					GetTagInfo(file, &hi);
					GetTotalPrice(file, &hi);
					GetUnitPrice(file, &hi);
					WriteCsv(out, &hi);
					divbegin = false;
				}
			}
		}
		fclose(out);
		fclose(file);
	}
	return 0;
}

bool GetTitleInfo(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "<div class=\"title\">")) {
			start = true;
			continue;
		}
		else if (strstr(buf, "</div>")) {
			if (start) {
				break;
			}
		}

		if (start) {
			if (strstr(buf, "title=")) {
				GetInfo("title=", &hi->title, buf);
			}
			else if (strstr(buf, "href=")) {
				GetInfo("href=", &hi->url, buf);
				size_t pos1 = hi->url.rfind('/');
				size_t pos2 = hi->url.rfind('.');
				if (pos1 != std::string::npos &&
					pos2 != std::string::npos &&
					pos1 < pos2) {
					hi->id = hi->url.substr(pos1 + 1, pos2 - pos1 - 1);
				}
			}
		}
	}
	return true;
}

bool GetHouseInfo(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "houseIcon")) {
			fgets(buf, 4096, file);
			if (buf[strlen(buf) - 2] == '\r') {
				buf[strlen(buf) - 2] = 0;
			}
			else {
				buf[strlen(buf) - 1] = 0;
			}
			string temp(buf);
			int pos = temp.find_first_not_of(' ');
			hi->info += temp.substr(pos);

			fgets(buf, 4096, file);
			if (buf[strlen(buf) - 2] == '\r') {
				buf[strlen(buf) - 2] = 0;
			}
			else {
				buf[strlen(buf) - 1] = 0;
			}
			temp.assign(buf);
			pos = temp.find_first_not_of(' ');
			hi->info += temp.substr(pos);

			fgets(buf, 4096, file);
			if (buf[strlen(buf) - 2] == '\r') {
				buf[strlen(buf) - 2] = 0;
			}
			else {
				buf[strlen(buf) - 1] = 0;
			}
			temp.assign(buf);
			pos = temp.find_first_not_of(' ');
			hi->info += temp.substr(pos);

			fgets(buf, 4096, file);
			if (buf[strlen(buf) - 2] == '\r') {
				buf[strlen(buf) - 2] = 0;
			}
			else {
				buf[strlen(buf) - 1] = 0;
			}
			temp.assign(buf);
			pos = temp.find_first_not_of(' ');
			hi->info += temp.substr(pos);

			break;
		}
	}
	return true;
}

bool GetFollowInfo(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "starIcon")) {
			fgets(buf, 4096, file);
			fgets(buf, 4096, file);
			char *ptr = strchr(buf, '/');
			char *end = ptr ? strchr(ptr + 2, ' ') : NULL;
			if (end) {
				ptr += 2;
				*end = 0;
				hi->follow.assign(ptr);
			}
			break;
		}
	}
	return true;
}

bool GetTagInfo(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "div class=\"tag\"")) {
			start = true;
			continue;
		}
		else if (strstr(buf, "</div>")) {
			if (start) {
				break;
			}
		}

		if (start) {
			if (strstr(buf, "taxfree")) {
				GetDivInfo("taxfree", &hi->taxfree, buf);
			}
			else if (strstr(buf, "new")) {
				GetDivInfo("new", &hi->taxfree, buf);
			}
		}
	}
	return true;
}

bool GetTotalPrice(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "totalPrice2")) {
			start = true;
			continue;
		}
		else if (strstr(buf, "</div>")) {
			if (start) {
				break;
			}
		}
		if (start) {
			if (strstr(buf, "span")) {
				GetDivInfo(&hi->totalprice, buf);
			}
		}
	}
}

bool GetUnitPrice(FILE *file, HouseInfo *hi)
{
	char buf[4097] = {0};
	memset(buf, 0, 4096);
	bool start = false;
	while (!feof(file)) {
		fgets(buf, 4096, file);
		if (strstr(buf, "unitPrice")) {
			start = true;
			continue;
		}
		else if (strstr(buf, "</div>")) {
			if (start) {
				break;
			}
		}
		if (start) {
			if (strstr(buf, "span")) {
				GetDivInfo(&hi->unitprice, buf);
				size_t found = hi->unitprice.find(',');
				if (found != std::string::npos) {
					hi->unitprice.erase(found, 1);
				}
			}
		}
	}
}

void GetInfo(const char *key, std::string *str, char *buf)
{
	char *ptr = strstr(buf, key);
	ptr = ptr ? strchr(ptr, '\"') : NULL;
	char *end = ptr ? strchr(ptr + 1, '\"') : NULL;
	if (end) {
		ptr++;
		*end = 0;
		str->assign(ptr);
	}
}

void GetDivInfo(const char *key, std::string *str, char *buf)
{
	char *ptr = strstr(buf, key);
	ptr = ptr ? strchr(ptr, '>') : NULL;
	char *end = ptr ? strchr(ptr, '<') : NULL;
	if (end) {
		*end = 0;
		if (str->empty()) {
			str->assign(++ptr);
		}
		else {
			*ptr = ' ';
			str->append(ptr);
		}
	}
}

void GetDivInfo(std::string *str, char *buf)
{
	char *ptr = strchr(buf, '>');
	char *end = ptr ? strchr(ptr, '<') : NULL;
	if (end) {
		ptr++;
		*end = 0;
		str->assign(ptr);
	}
}

void WriteCsv(FILE *file, const HouseInfo *hi)
{
	fwrite(hi->id.c_str(), hi->id.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->totalprice.c_str(), hi->totalprice.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->unitprice.c_str(), hi->unitprice.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->info.c_str(), hi->info.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->title.c_str(), hi->title.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->follow.c_str(), hi->follow.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->taxfree.c_str(), hi->taxfree.length(), 1, file);
	fwrite(",", 1, 1, file);
	fwrite(hi->url.c_str(), hi->url.length(), 1, file);
	fwrite("\n", 1, 1, file);
}


