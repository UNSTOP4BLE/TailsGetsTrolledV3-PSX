#define XA_LENGTH(x) (((u64)(x) * 75) * IO_SECT_SIZE) //Seconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(113)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(38)},  //XA_GameOver
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(170)}, //XA_Bopeebo
	{XA_Week1A, XA_LENGTH(165)}, //XA_Fresh
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(171)}, //XA_Dadbattle
	//WEEK2A.XA
	{XA_Week2A, XA_LENGTH(144)}, //XA_Spookeez
	{XA_Week2A, XA_LENGTH(174)}, //XA_South
	//WEEK2B.XA
	{XA_Week2B, XA_LENGTH(252)}, //XA_Monster
	//WEEK3A.XA
	{XA_Week3A, XA_LENGTH(149)},  //XA_Pico
	{XA_Week3A, XA_LENGTH(169)}, //XA_Philly
	//WEEK3B.XA
	{XA_Week3B, XA_LENGTH(170)}, //XA_Blammed
	//WEEK4A.XA
	{XA_Week4A, XA_LENGTH(169)},  //XA_SatinPanties
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	"\\MUSIC\\WEEK2A.XA;1", //XA_Week2A
	"\\MUSIC\\WEEK2B.XA;1", //XA_Week2B
	"\\MUSIC\\WEEK3A.XA;1", //XA_Week3A
	"\\MUSIC\\WEEK3B.XA;1", //XA_Week3B
	"\\MUSIC\\WEEK4A.XA;1", //XA_Week4A
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	//WEEK1A.XA
	{"bopeebo", true}, //XA_Bopeebo
	{"fresh", true},   //XA_Fresh
	//WEEK1B.XA
	{"dadbattle", true}, //XA_Dadbattle
	//WEEK2A.XA
	{"spookeez", true}, //XA_Spookeez
	{"south", true},    //XA_South
	//WEEK2B.XA
	{"monster", true}, //XA_Monster
	//WEEK3A.XA
	{"pico", true},   //XA_Pico
	{"philly", true}, //XA_Philly
	//WEEK3B.XA
	{"blammed", true}, //XA_Blammed
	//WEEK4A.XA
	{"satinpanties", true}, //XA_SatinPanties
	
	{NULL, false}
};
