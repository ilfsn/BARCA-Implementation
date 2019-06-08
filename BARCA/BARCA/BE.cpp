#include "BE.h"
#include <math.h>
static int consecutive = 0;

static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

std::vector<double> feature_4extr(std::vector<std::vector<double>> data) {
	
	int n_metric = data[0].size();

	std::vector<double> ME = std::vector<double>(n_metric, 0);
	std::vector<double> SD = std::vector<double>(n_metric, 0);
	std::vector<double> SK = std::vector<double>(n_metric, 0);
	std::vector<double> KU = std::vector<double>(n_metric, 0);
	
	for (auto r : data) {
		for (int i = 0; i < n_metric; i++) ME[i] += r[i];
	}
	for (int i = 0; i < n_metric; i++) ME[i] /= data.size();
	///////////////////////////////////////////////////////////
	
	for (auto r : data) {
		for (int i = 0; i < n_metric; i++) {
			SD[i] += pow(r[i] - ME[i], 2);
			SK[i] += pow(r[i] - ME[i], 3);
			KU[i] += pow(r[i] - ME[i], 4);
		}
	}
	for (int i = 0; i < n_metric; i++) {
		SD[i] = sqrt(SD[i] / data.size());
		if (SD[i] == 0) {
			SK[i] = 0;
			KU[i] = 0;
		}
		else {
			SK[i] = SK[i] / (data.size()*pow(SD[i], 3));
			KU[i] = KU[i] / (data.size()*pow(SD[i], 4)) - 3;
		}
	}
	///////////////////////////////////////////////////////////
	std::vector<double> ret;
	std::cout << "ME: ";
	for (auto r : ME) {
		std::cout << r << " , ";
		ret.push_back(r);
	}
	std::cout << std::endl;

	std::cout << "SD: ";
	for (auto r : SD) {
		std::cout << r << " , ";
		ret.push_back(r);
	}
	std::cout << std::endl;

	std::cout << "SK: ";
	for (auto r : SK) {
		std::cout << r << " , ";
		ret.push_back(r);
	}
	std::cout << std::endl;

	std::cout << "KU: ";
	for (auto r : KU) {
		std::cout << r << " , ";
		ret.push_back(r);
	}
	std::cout << std::endl;

	return ret;
}

std::vector<std::vector<double>> get_sequence(sqlite3* DB, int window_z, int n_metric, std::vector<std::string> list_name) {
	std::vector<std::vector<double>> ret;
	char sql_buffer[128];
	sqlite3_stmt* stmt;
	
	for (auto v : list_name) {
		sprintf_s(sql_buffer, "SELECT * FROM %s WHERE rowid BETWEEN %d AND %d;", v.c_str(), consecutive + 1, consecutive + window_z);
		int rc = sqlite3_prepare_v2(DB, sql_buffer,strlen(sql_buffer) , &stmt, NULL);
		for (;;) {
			rc = sqlite3_step(stmt);
			if (rc == SQLITE_DONE)
				break;
			if (rc != SQLITE_ROW) {
				printf("error: %s!\n", sqlite3_errmsg(DB));
				break;
			}
			
			std::vector<double> vt_temp;
			for (int i = 0; i < n_metric; i++) {
				vt_temp.push_back(sqlite3_column_double(stmt, i));
			}
			ret.push_back(vt_temp);
		}
	}
	consecutive += window_z;
	return ret;
}

int BehEx::get_table_name() {
	sqlite3_stmt* t_statement;
	const char *sql;
	sql = "SELECT name FROM sqlite_master WHERE type='table';";
	size_t t_len = strlen(sql);
	char *zErrMsg = 0;

	int rc = sqlite3_prepare_v2(DB, sql, t_len, &t_statement, NULL);

	for (;;) {
		rc = sqlite3_step(t_statement);
		if (rc == SQLITE_DONE)
			break;
		if (rc != SQLITE_ROW) {
			printf("error: %s!\n", sqlite3_errmsg(DB));
			break;
		}

		const char *text = (const char *)sqlite3_column_text(t_statement, 0);
		list_name.push_back(std::string(text));

		/*MyData d;
		const char *text = (const char *)sqlite3_column_text(t_statement, 0);
		d.A = text ? text : "";
		d.B = sqlite3_column_int(stmt, 1);
		d.C = sqlite3_column_int(stmt, 2);
		list.push_back(d);*/
	}

	return 0;
}


std::vector<double> BehEx::read_metric()
{
	std::vector<std::vector<double>> vectors = get_sequence(DB, size_window, n_metric, list_name);
	for (auto v : vectors) {
		for (auto i : v) {
			std::cout << i << " , ";
		}
		std::cout << std::endl;
	}

	return feature_4extr(vectors);
}

std::vector<double> BehEx::feature_extract()
{
	return std::vector<double>();
}
