/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* Constants */
#define TERMINAL "st"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh = 12; /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "scientifica:size=8","Misc Stlarch:size=8", "Siji:size=8","Noto Emoji:size=9", };
static const char dmenufont[]       = "scientifica:size=8";
static const char col_bg[]          = "#fbf3db";
static const char col_fg[]          = "#53676d";
static const char col_sel[]         = "#006dce";
static const char *colors[7][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg, col_bg, col_bg },
	[SchemeSel]  = { col_bg, col_sel,  col_sel  },
    [SchemeInfo]  = { col_fg, "#e9e4d0",  col_sel  },
};
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor    scratch key */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1,         0  },
	{ NULL,		 NULL,     "umpv",         0,         1,          0,           0,        -1,        'u'  },
	{ "st",		 NULL,     NULL,           0,         0,          1,		   0,        -1,         0  },
	{ "st",		 NULL,     "floating",     0,         1,          1,          -1,        -1,         0  },
	{ NULL,      NULL,     "Event Tester", 0,         1,          0,           1,        -1,         0  }, /* xev */
	{ NULL,      NULL,     "scratchpad",   0,         1,          0,           0,        -1,        's' },
	{ NULL,      NULL,     "ncmpcpp",      0,         1,          0,           0,        -1,        'n' },
};



/* layout(s) */
static const float mfact     = 0.52; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ Mod1Mask|ShiftMask, 			KEY,      tagunfocused,   {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERMINAL, NULL };


#include "movestack.c"
/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "st", "-t", "scratchpad", NULL};
static const char *scratchpadcmdmusic[] = {"n", "st", "-t", "ncmpcpp", "music", NULL};
static const char *scratchpadumpv[] = {"u", "umpv", "/home/cassio/pics/photos/background.png", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             		    XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_o,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_m,      togglescratch,  {.v = scratchpadcmdmusic } },
	{ MODKEY,                       XK_u,      togglescratch,  {.v = scratchpadumpv } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_b,      spawn,      	   SHCMD("$BROWSER > /dev/null") },
	{ MODKEY, 			            XK_c,      spawn,      	   SHCMD("LC_ALL=pt_BR.utf8 " TERMINAL " -e calcurse") },
	{ MODKEY, 			            XK_e,      spawn,      	   SHCMD(TERMINAL " -e lf") },
	{ MODKEY, 			            XK_f,      spawn,      	   SHCMD("dmenumount") },
	{ MODKEY, 			            XK_g,      spawn,      	   SHCMD(TERMINAL " -t 'floating' -e htop -t -s PERCENT_CPU") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,      	   SHCMD(TERMINAL " -e neomutt") },
	{ MODKEY, 			            XK_n,      spawn,      	   SHCMD("bookmarksman") },
	{ MODKEY, 						XK_q,      spawn,      	   SHCMD("configmenu") },
	{ MODKEY, 			            XK_r,      spawn,      	   SHCMD(TERMINAL " -e newsboat") },
	{ MODKEY, 			            XK_s,      spawn,      	   SHCMD("mountpi") },
	{ MODKEY, 			            XK_t,      spawn,      	   SHCMD(TERMINAL " -e tremc") },
	{ MODKEY|ShiftMask, 			XK_x,      spawn,      	   SHCMD("powermenu") },
	{ MODKEY,						XK_Up,	   spawn,		   SHCMD("xbacklight -inc 1 -steps 1 -time 0") },
	{ MODKEY,						XK_Down,   spawn,		   SHCMD("xbacklight -dec 1 -steps 1 -time 0") },
	{ MODKEY,						XK_Print,  spawn,		   SHCMD("scrot -e 'mv $f ~/pics/screenshots/'") },
	{ MODKEY,						XK_F1,	   spawn,		   SHCMD("nowplaying") },
	{ 0, XF86XK_AudioMute,			spawn,	   SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMicMute,		spawn,	   SHCMD("pamixer -t --source 1") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,	   SHCMD("pamixer -i 5; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,	   SHCMD("pamixer -d 5; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,			spawn,	   SHCMD("mpc prev; kill -46 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioNext,			spawn,	   SHCMD("mpc next; kill -46 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPlay,			spawn,	   SHCMD("mpc toggle; kill -46 $(pidof dwmblocks)") },
	{ 0, XF86XK_Launch1,			spawn,	   SHCMD(TERMINAL " -t floating -e pulsemixer") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,	   SHCMD("xbacklight -inc 5") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,	   SHCMD("xbacklight -dec 5") },
	{ 0, XF86XK_ScreenSaver,		spawn,	   SHCMD("slock") },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.015} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.015} },
    { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             		    XK_w,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_j,      cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_k,      cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,						XK_space,  spawn,	       SHCMD("mpc toggle; kill -46 $(pidof dwmblocks)") },
	{ MODKEY|Mod1Mask,              XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod1Mask,		        XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

