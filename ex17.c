#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address
{
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database
{
	struct Address rows[MAX_ROWS];
};

struct Connection
{
	FILE* file;
	struct Database* db;
};

void Die(const char* message, Const char* conn)
{
	if(errno)
	{
		perror(message);
	}
	else
	{
		printf("ERROR: %s\n", message);
	}
	DatabaseClose(conn);
	exit(1);
}

void AddressPrint(struct Address* addr)
{
	printf("%d %s %s\n",
		addr->id, addr->name, addr->email);
}

void DatabaseLoad(struct Connection* conn)
{
	int rc = fread(
		conn->db,
		sizeof(struct Database),
		1,
		conn->file);
	if(rc != 1) Die("Failed to load db.", conn);
}
 
struct Connection* DatabaseOpen(const char* filename,
char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn) Die("Memory Error!", conn);

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) Die("Memory Error!", conn);

	if(mode == 'c')
	{
		conn->file = fopen(filename, "w");
	}
	else
	{
		conn->file = fopen(filename, "r+");
		if(conn->file)	DatabaseLoad(conn);
	}

	if(!conn->file) Die("Failed to open the file!", conn);

	return conn;
}//end of function databaseopen

void DatabaseClose(struct Connection* conn)
{
	if(conn)
	{
		if(conn->file) fclose(conn->file);
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void DatabaseWrite(struct Connection* conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) Die("Die failed to write to database!", conn);

	rc = fflush(conn->file);
	if(rc == -1) Die("Cannot flush database!", conn);
}

void DatabaseCreate(struct Connection* conn )
{
	int i = 0;

	for(i = 0; i < MAX_ROWS; i++)
	{
		//make a prototype to initialise it
		struct Address addr = {.id = i, .set =0};
		//then just assign it
		conn->db->rows[i] = addr;
	}
}

void DatabaseSet(struct Connection* conn, int id, const char* name, const char* email)
{
	struct Address* addr = &conn->db->rows[id];
	if(addr->set) Die("Already set, deleted it first!", conn);

	addr->set =1;
	//WARNING: bug, read the "How to break it" section and fix this
	char* res = strncpy(addr->name,name,MAX_DATA);
	//demonstrate the strncpy bug
	if(!res) Die("Name copy failed", conn);

	res = strncpy(addr->email,email,MAX_DATA);
	if(!res) Die("Email copy failed", conn);
}

void DatabaseGet(struct Connection* conn, int id)
{
	struct Address* addr = &conn->db->rows[id];

	if(addr->set)
	{
		AddressPrint(addr);
	}
	else
	{
		Die("Id is not set.", conn);
	}
}

void DatabaseDelete(struct Connection* conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void DatabaseList(struct Connection* conn)
{
	int i = 0;
	struct Database* db = conn->db;

	for(i=0;i<MAX_ROWS;i++)
	{
		struct Address* cur = &db->rows[i];
		if(cur->set) AddressPrint(cur);
	}
}

int main(int argc, char* argv[])
{
	if(argc<3) Die("USAGE: ex17 <dbfile> <action> [action params]", conn);

	char* filename = argv[1];
	char action = argv[2][0];
	struct Connection* conn = DatabaseOpen(filename, action);
	int id =0;

	if(argc>3) id = atoi(argv[3]);
	if(id>=MAX_ROWS) Die("There's not that many records.", conn);

	switch(action)
	{
		case 'c':
			DatabaseCreate(conn);
			DatabaseWrite(conn);
			break;

		case 'g':
			if(argc != 4) Die("Need an id to get", conn);

			DatabaseGet(conn, id);
			break;

		case 's':
			if(argc != 6) Die("Need id, name, email to set.", conn);

			DatabaseSet(conn, id, argv[4], argv[5]);
			DatabaseWrite(conn);
			break;

		case 'd':
			if(argc != 4) Die("Need id to delete", conn);

			DatabaseDelete(conn, id);
			DatabaseWrite(conn);
			break;

		case 'l':
			DatabaseList(conn);
			break;
		default:
			Die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}//end switch action

	DatabaseClose(conn);

	return 0;
}//end main
