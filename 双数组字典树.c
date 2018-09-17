/*************************************************************************
	> File Name: 双数组字典树.c
	> Author: 史桢港 
	> Mail: 1031091451@qq.com
	> Created Time: 2018年02月02日 星期五 08时52分38秒
 ************************************************************************/
//在工业中经常使用
//是线下构建，然后在线上使用
//有较高的查询效率


//可以用字典树构建， 层次遍历
//递归构建

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode {
    int flag;
    struct TrieNode *next[26];
} TrieNode, *Trie;

typedef struct DoubleArrayTrieNode {
    int n;
    int *base, *check;
} DoubleArrayTrieNode, *DATrie;

DATrie initDATrie(int n) {
    DoubleArrayTrieNode *p = (DATrie)malloc(sizeof(DoubleArrayTrieNode));
    p->base = (int *)calloc(n, sizeof(int));
    p->check = (int *)calloc(n, sizeof(int));
    p->check[1] = -1;
    p->n = n;
    return p;
}

TrieNode *getNewNode() {
    TrieNode *p = (TrieNode *)malloc(sizeof(TrieNode));
    p->flag = 0;
    for (int i = 0; i < 26; ++i) {
        p->next[i] = NULL;
    }
    return p;
} 

int insert(Trie tree, const char *str) {
    TrieNode *p = tree;
    int times = 0;
    while (str[0]) {
        if (p->next[str[0] - 'a'] == NULL) {
            p->next[str[0] - 'a'] = getNewNode();
            times++;
        }
        p = p->next[str[0] - 'a'];
        str++;
    }
    p->flag = 1;
    return times;
} 

int search(Trie tree, const char *str) {
    TrieNode *p = tree;
    while(str[0] && p) {
        p = p->next[str[0] - 'a'];
        str++;
    }
    return p && p->flag;
}

void clear(Trie tree) {
    if (tree == NULL) return;
    for (int i = 0; i < 26; ++i) {
        if (tree->next[i] == NULL) continue;
        clear(tree->next[i]);
    }
    free(tree);
}

int transToDATrie(Trie ftree, int ind, DATrie ttree) {
    int val = 0, flag = 1, ret = ind;
    while (flag) {
        flag = 0;
        val += 1;
        for (int i = 0; i < 26; ++i) {
            if (ftree->next[i] == NULL) continue;
            if (ttree->check[val + i] == 0) continue;
            flag = 1;
            break;
        }
        
    }
    if (ftree->flag) ttree->check[ind] = -ttree->check[ind];
    ttree->base[ind] = val;
    for (int i = 0; i < 26; ++i) {
        if (ftree->next[i] == NULL) continue;
        ttree->check[val + i] = ind;
    }
    for (int i = 0; i < 26; ++i) {
        if (ftree->next[i] == NULL) continue;
        int temp = transToDATrie(ftree->next[i], val + i, ttree);
        ret = (ret > temp ? ret : temp);
    }
    return ret;

}// 在转换过程中的最大坐标值

void clearDATrie(DATrie tree) {
    if (tree == NULL) return ;
    free(tree->base);
    free(tree->check);
    free(tree);
    return ;
}

int main() {
    int n, m, node_cnt = 0;;
    scanf("%d %d", &n, &m);
    char str[100];
    Trie tree = getNewNode();
    while (n--) {
        scanf("%s", str);
        node_cnt += insert(tree, str);
    }
    DATrie da_tree = initDATrie(2 * node_cnt);
    int max_node_ind = transToDATrie(tree, 1, da_tree);
    for (int i = 0; i <= max_node_ind; ++i) {
        printf("(%4d%4d%4d) ", i, da_tree->base[i], da_tree->check[i]);
        if (i % 5 == 0) printf("\n");
    }
    printf("\n");
    while (m--) {
        scanf("%s", str);
       printf("%d : %s\n", search(tree, str), str);
    }

    return 0;
}
