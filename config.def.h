/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Open Sans:size=10" };
static const char dmenufont[]       = "Open Sans:size=10";

#include "themes/mocha-mauve.h"

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { white,     black,     gray2 },
	[SchemeSel]  = { gray4,     accent,    accent  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance   title     tags mask   isfloating   monitor */
	{ "Google-chrome",      NULL,   NULL,       1 << 1,           0,       -1 },
	{ "obsidian"     ,      NULL,   NULL,       1 << 5,           0,       -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle }, /* first entry is default */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	
};

/* key definitions */
#define MODKEY Mod4Mask //WinKey
#define ALT Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", gray2, "-nf", white, "-sb", accent, "-sf", gray4, NULL };
static const char *termcmd[]  = { "xterm", NULL };
static const char *obsdcmd[]  = { "obsidian", NULL };
static const char *fmcmd[] = { "spacefm", NULL };
static const char *scrotcmd[]  = { "make-screenshot.sh", NULL };
static const char *incr_sound[]  = { "pulsemixer", "--change-volume", "+5",	 NULL };
static const char *decr_sound[]	 = { "pulsemixer", "--change-volume", "-5",	 NULL };
static const char *mute_sound[]  = { "pulsemixer", "--toggle-mute", NULL };
static const char *incr_brightness[]  = { "/bin/sh", "-c", "~/.scripts/incr_brightness.sh", NULL };
static const char *decr_brightness[]  = { "/bin/sh", "-c", "~/.scripts/decr_brightness.sh", NULL };
static const char *rotate_scree_right[] = {"xrandr", "--output", "eDP1", "--rotate", "left", NULL };
static const char *rotate_scree_left[] = {"xrandr", "--output", "eDP1", "--rotate", "right", NULL };
static const char *rotate_scree_normal[] = {"xrandr", "--output", "eDP1", "--rotate", "normal", NULL };
static const char *rotate_scree_upsidedown[] = {"xrandr", "--output", "eDP1", "--rotate", "inverted", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },                        //choose to launch a program
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },                         //Terminal
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY|ControlMask,           XK_Return, spawn,          {.v = fmcmd } },                           //Explorer
    { MODKEY,                       XK_o,      spawn,          {.v = obsdcmd } },                         //Obsidian
	{ 0,                            XK_Print,  spawn,          {.v = scrotcmd } },                        //screenshot
	{ MODKEY,                       XK_b,      togglebar,      {0} },                                     //Show/hide status
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },                              //Focus on a widow above in stack
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },                              //Focus on a widow below in stack
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },                              //Increase number of master windows
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },                              //Decrease number of master windows
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },                            //Move border it Tile mode left
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },                            //Move border it Tile mode right
	{ MODKEY,                       XK_Return, zoom,           {0} },                                     //Move widow on top of the stack
	{ MODKEY,                       XK_Tab,    view,           {0} },                                     //go to previous Tag
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },                                     //close window
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },                      //Tile layout
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },                      //Float layout
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },                      //Monocle layout
	{ MODKEY,                       XK_space,  setlayout,      {0} },                                     //go to previous layout
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },                                     //Make focused window in floating mode
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },                             //Selects all the tags
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },                             //Assign window all tags
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },                              // 
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },                              //
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },                              //
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },                              //
	{ MODKEY,                       XK_Right,  spawn,          {.v = rotate_scree_right} },               //
	{ MODKEY,                       XK_Left,   spawn,          {.v = rotate_scree_left} },                //
	{ MODKEY,                       XK_Up,     spawn,          {.v = rotate_scree_upsidedown } },              //
	{ MODKEY,                       XK_Down,   spawn,          {.v = rotate_scree_normal } },          //
	{ MODKEY,                       XK_F4,     quit,           {0} },                                     //close dwm
	{ 0,                            XF86XK_MonBrightnessUp,    spawn,          {.v = incr_brightness } }, //
	{ 0,       					    XF86XK_MonBrightnessDown,  spawn,          {.v = decr_brightness } }, //
	{ 0,          					XF86XK_AudioRaiseVolume,   spawn,          {.v = incr_sound } },      //
	{ 0,	      					XF86XK_AudioLowerVolume,   spawn,	       {.v = decr_sound } },      //
	{ 0,          					XF86XK_AudioMute,          spawn,          {.v = mute_sound } },        //
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

