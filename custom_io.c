#include <stdio.h>
#include "binary_tree.h"
#include "custom_io.h"

#define MARKER -1
#define MAX_LINE_SIZE 100
#define BUF_DATA_SIZE 50

/* prototypes for static functions */
static void serialize_preorder(BT_Node *root, FILE *fp);
static void add_to_file(char *filename, char *to_write);
static void inorder_to_file(BT_Node *root, FILE *fp);

/* Writes a binary tree to file
void serialize_tree(Binary_Tree *tree, char *filename) {
	FILE *fp;
	fp = fopen(filename, "w");
	serialize_preorder(tree->root, fp);
	fclose(fp);
}

/* Writes an entire binary tree to file, preorder */
static void serialize_preorder(BT_Node *root, FILE *fp) {
	if (root == NULL) {
		fprintf(fp, "%d,%d\n", MARKER, MARKER);
		return;
	}

	fprintf(fp, "%d,%s\n", root->key, root->data);
	serialize_preorder(root->left_child, fp);
	serialize_preorder(root->right_child, fp);
}

/* Reads an entire binary tree to file, expects preorder layout */
BT_Node* deserialize_preorder(FILE *fp) {
	int key;
	char buf[BUF_DATA_SIZE];

	if (!fscanf(fp, "%d,%s", &key, buf) || key == MARKER) {
		return NULL;
	}

	printf("Read in: %d, %s", key, buf);
	BT_Node *root = create_bt_node(key, buf);
	root->left_child = deserialize_preorder(fp);
	root->right_child = deserialize_preorder(fp);

	return root;
}

/* Compact files -- use a merge sort approach merge log files together */
void compaction(char *filename1, char *filename2) {
	// TO DO
}


/* Writes a Tree to Log File (no Binary Tree structure maintained) */
void tree_to_log_file(Binary_Tree *tree, char *filename) {
	FILE *fp;
	fp = fopen(filename, "w");
	inorder_write(tree->root, fp);
	fclose(fp);
}

/* Takes a tree root, traverses tree "inorder" in order to add
 * tree data, ordered by key */
static void inorder_to_file(BT_Node *root, FILE *fp) {
	// we've traversed past a root
	if (root == NULL) {
		return;
	}
	inorder_to_file(root->left, fp);
	fprintf(fp, "%d,%s\n", root->key, root->value);
	inorder_to_file(root->right, fp);
}

/* Writes key, value pair to write ahead log */
void kv_to_wal(char *wal, int key, char *value) {
	char to_write[MAX_LINE_SIZE];
	sprintf(to_write, "%d,%s", key, value);
	add_to_file(wal, to_write);
}

/* Append to file */
static void add_to_file(char *filename, char *to_write) {
	FILE *fp;
	fp = fopen(filename, "a");

	fprintf(fp, "%s\n", to_write);
	fclose(fp);
}

/* Deletes contents of a file identified by filename,
 * but keeps the file itself */
void delete_file_contents(char *filename) {
	FILE *fp;
	fp = fopen(filename, "w");
	fclose(fp);
}

