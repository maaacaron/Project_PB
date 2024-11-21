#define MAX_STORY_LENGTH 10000
#define MAX_SELECT_LENGTH 500
#define MAX_NORMALEVENT_NUM 20

struct price
{
    int exp;
    int HP;
    int AP;
    int DP;
    int SP;
    int skill;
};

struct select
{
    char choice[MAX_SELECT_LENGTH];
    struct price prices;
};


struct event
{
    int event_id;
    int choice_num;
    char story[MAX_STORY_LENGTH];

    struct select select_1;
    struct select select_2;
    struct select select_3;
    struct select select_4;
    struct select select_5;
};