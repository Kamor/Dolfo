/*
    Daimonin, the Massive Multiuser Online Role Playing Game
    Server Applicatiom

    Copyright (C) 2001 Michael Toennies

    A split from Crossfire, a Multiplayer game for X-windows.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    The author can be reached via e-mail to info@daimonin.org
*/

/* TREASURE_DEBUG does some checking on the treasure lists after loading.
 * It is useful for finding bugs in the treasures file.  Since it only
 * slows the startup some (and not actual game play), it is by default
 * left on
 */
#define TREASURE_DEBUG

/* TREASURE_VERBOSE enables copious output concerning artifact generation */
/*#define TREASURE_VERBOSE*/

#include <global.h>

#define PUT(_O_, _C_, _F_) \
    if (((_F_) & GT_ENVIRONMENT)) \
    { \
        (_O_)->x = (_C_)->x; \
        (_O_)->y = (_C_)->y; \
        (void)insert_ob_in_map((_O_), (_C_)->map, (_O_), INS_NO_WALK_ON); \
    } \
    else \
    { \
        (void)insert_ob_in_ob((_O_), (_C_)); \
    }

/* static functions */
static treasure_t        *load_treasure(FILE *fp, int *t_style, int *a_chance);
static void             change_treasure(struct _change_arch *ca, object_t *op); /* overrule default values */
static treasurelist_t    *get_empty_treasurelist(void);
static treasure_t        *get_empty_treasure(void);
static inline void      set_material_real(object_t *op, struct _change_arch *change_arch);
static void             create_money_table(void);
static void             postparse_treasurelist(treasure_t *t, treasurelist_t *tl);

/*
 * Opens LIBDIR/treasure and reads all treasure-declarations from it.
 * Each treasure is parsed with the help of load_treasure().
 */

static void init_treasures(FILE *fp)
{
    static treasurelist_t    *previous = NULL; /* important, we call this now recursive */
    char                    buf[MEDIUM_BUF], name[MEDIUM_BUF];
    treasurelist_t           *tl_tmp;
    treasure_t               *t;
    int                     t_style, a_chance;
    char                    dummy[10];


    while (fgets(buf, MEDIUM_BUF, fp) != NULL)
    {
        if (*buf == '#' || *buf == '\n' || *buf == '\r')
            continue;
        if (sscanf(buf, "treasureone %s%[\n\r]", name, dummy) || sscanf(buf, "treasure %s%[\n\r]", name, dummy))
        {
            treasurelist_t   *tl  = get_empty_treasurelist();
            SHSTR_FREE_AND_ADD_STRING(tl->listname, name);
            /* check for double used list name */
            for (tl_tmp = first_treasurelist; tl_tmp != NULL; tl_tmp = tl_tmp->next)
            {
                if (tl->listname == tl_tmp->listname)
                    break;
            }
            if (tl_tmp)
            {
                LOG(llevError, "ERROR: Treasure list name <%s> double used.\n", STRING_SAFE(tl->listname));
                FREE(tl);

                return;
            }

            if (previous == NULL)
                first_treasurelist = tl;
            else
                previous->next = tl;
            previous = tl;
            t_style = T_STYLE_UNSET;
            a_chance = ART_CHANCE_UNSET;
            tl->items = load_treasure(fp, &t_style, &a_chance);
            if (tl->t_style == T_STYLE_UNSET)
                tl->t_style = t_style;
            if (tl->artifact_chance == ART_CHANCE_UNSET)
                tl->artifact_chance = a_chance;
            /* This is a one of the many items on the list should be generated.
             * Add up the chance total, and check to make sure the yes & no
             * fields of the treasures are not being used.
             */
            if (!strncmp(buf, "treasureone", 11))
            {
                for (t = tl->items; t != NULL; t = t->next)
                {
#ifdef TREASURE_DEBUG
                    if (t->next_yes || t->next_no)
                    {
                        LOG(llevBug, "BUG: Treasure %s is one item, but on treasure %s\n", tl->listname,
                            t->item ? t->item->name : t->name);
                        LOG(llevBug, "BUG:  the next_yes or next_no field is set");
                    }
#endif
                    tl->total_chance += t->chance;
                }
#if 0
        LOG(llevDebug, "Total chance for list %s is %d\n", tl->name, tl->total_chance);
#endif
            }
        }
        else
            LOG(llevError, "ERROR: Treasure-list didn't understand: %s\n", buf);
    }
}

/*
* recusively traverse the given directory and search for *.tl files
* and process them like they are part of the original treasure list file
*/
static void traverse_treasures_files(char* start_dir)
{
    DIR* dir;                    /* pointer to the scanned directory. */
    struct dirent* entry=NULL;   /* pointer to one directory entry.   */
    char *fptr, cwd[HUGE_BUF+1]; /* current working directory.        */
    struct stat dir_stat;        /* used by stat().                   */

    /* first, save path of current working directory */
    if (!getcwd(cwd, HUGE_BUF+1)) {
        perror("getcwd:");
        return;
    }

    /* open the directory for reading */
    if(start_dir)
    {
        if ((dir = opendir(start_dir)))
        {
            if (chdir(start_dir) == -1)
            {
                fprintf(stderr, "Cannot chdir into '%s': ", start_dir);
                perror("");
                dir = NULL;
            }
        }
    }
    else
        dir = opendir(".");

    if (!dir) {
        fprintf(stderr, "Cannot read directory '%s': ", cwd);
        perror("");
        return;
    }

    /* scan the directory, traversing each sub-directory, and */
    /* matching the pattern for each file name.               */
    while ((entry = readdir(dir)))
    {
        /* check if the given entry is a directory. */
        /* skip all ".*" entries, to avoid loops and forbidden directories. */
        if (entry->d_name[0] == '.')
            continue;

        if (stat(entry->d_name, &dir_stat) == -1)
        {
            perror("stat:");
            continue;
        }

        /* is this a directory? */
        if (S_ISDIR(dir_stat.st_mode))
        {
            /* Change into the new directory */
            if (chdir(entry->d_name) == -1)
            {
                fprintf(stderr, "Cannot chdir into '%s': ", entry->d_name);
                perror("");
                continue;
            }
            /* check this directory */
            traverse_treasures_files(NULL);

            /* finally, restore the original working directory. */
            if (chdir("..") == -1)
            {
                fprintf(stderr, "Cannot chdir back to '%s': ", cwd);
                perror("");
            }
        }
        else
        {
            /* lets check its a valid, local artifacts file */
            if(entry->d_name[0] != '.' && (fptr = strrchr(entry->d_name, '.')) && !strcmp(fptr, ".tl") )
            {
                FILE *fp;

                LOG(llevDebug, " adding local treasures from %s...", entry->d_name);
                if ((fp = fopen(entry->d_name, "r")) == NULL)
                {
                    LOG(llevError, "ERROR: Can't open %s.\n", entry->d_name);
                    exit(SERVER_EXIT_TREASURE);
                }

                init_treasures(fp);
                fclose(fp);
                LOG(llevDebug, "done.\n");
            }
        }
    }

    closedir(dir);

    if(start_dir) /* clean restore */
    {
        if (chdir(cwd) == -1)
        {
            fprintf(stderr, "Cannot chdir into '%s': ", cwd);
            perror("");
        }
    }
}

void load_treasures(void)
{
    FILE                   *fp;
    char                    filename[MEDIUM_BUF];
    treasurelist_t           *previous = NULL;

    /* load default treasure list file from /lib */
    sprintf(filename, "%s/%s", settings.datadir, settings.treasures);
    if ((fp = fopen(filename,"r")) == NULL)
    {
        LOG(llevError, "ERROR: Can't open treasure file.\n");
        return;
    }
    init_treasures(fp);
    fclose(fp);

    /* traverse the /maps folder and load every file with .tl extension as local treasure list */
    traverse_treasures_files(settings.mapdir);

    LOG(llevInfo, " link treasure lists pass 2...\n");
    for (previous = first_treasurelist; previous != NULL; previous = previous->next)
        postparse_treasurelist(previous->items, previous);

    create_money_table();
}


static void postparse_treasurelist(treasure_t *t, treasurelist_t *tl)
{
    treasurelist_t   *tl_tmp;

    if (t->item == NULL && t->name == NULL)
        LOG(llevError, "ERROR: Treasurelist %s has element with no name or archetype\n", STRING_SAFE(tl->listname));
    if (t->chance >= 100 && t->next_yes && (t->next || t->next_no))
        LOG(llevBug,
            "BUG: Treasurelist %s has element that has 100%% generation, next_yes field as well as next or next_no\n",
            tl->listname);

    /* if we have a list name && its not "none" -> link the list in */
    if (t->name && t->name != shstr_cons.none)
    {
        for (tl_tmp = first_treasurelist; tl_tmp != NULL; tl_tmp = tl_tmp->next)
        {
            if (t->name == tl_tmp->listname)
                break;
        }
        if (!tl_tmp)
            LOG(llevError, "ERROR: Treasurelist %s has element with invalid name <%s>\n", STRING_SAFE(tl->listname),
                STRING_SAFE(t->name));

        t->tlist = tl_tmp;
    }

    if (t->next)
        postparse_treasurelist(t->next, tl);
    if (t->next_yes)
        postparse_treasurelist(t->next_yes, tl);
    if (t->next_no)
        postparse_treasurelist(t->next_no, tl);
}

/* to generate from a value a set of coins (like 3 gold, 4 silver and 19 copper)
 * we collect the archetype_t for it out of the arch name for faster access.
 */
static void create_money_table(void)
{
    coins_arch[0] = find_archetype("mitcoin");
    coins_arch[1] = find_archetype("goldcoin");
    coins_arch[2] = find_archetype("silvercoin");
    coins_arch[3] = find_archetype("coppercoin");
    coins_arch[4] = NULL;

    if (!coins_arch[0] || !coins_arch[1] || !coins_arch[2] || !coins_arch[3])
    {
        LOG(llevError, "create_money_table(): Can't find money.\n (mit: %p - gold: %p - silver: %p - copper: %p)",
                coins_arch[0],coins_arch[1],coins_arch[2],coins_arch[3]);
        return;
    }
}

/*
 * Reads the lib/treasure file from disk, and parses the contents
 * into an internal treasure structure (very linked lists)
 */
static treasure_t * load_treasure(FILE *fp, int *t_style, int *a_chance)
{
    char        buf[MEDIUM_BUF], *cp=NULL, variable[MEDIUM_BUF];
    treasure_t   *t   = get_empty_treasure();
    int         value;
    int         start_marker = 0, t_style2, a_chance2;

    nroftreasures++;
    while (fgets(buf, MEDIUM_BUF, fp) != NULL)
    {
        if (*buf == '#')
            continue;
        if ((cp = strchr(buf, '\n')) != NULL)
            *cp = '\0';
        cp = buf;
        while (!isalpha(*cp)) /* Skip blanks */
            cp++;
        if (sscanf(cp, "t_style %d", &value))
        {
            if (start_marker)
                t->t_style = value;
            else
            {
                *t_style = value; /* no, its global for the while treasure list entry */
            }
        }
        else if (sscanf(cp, "artifact_chance %d", &value))
        {
            if (start_marker)
                t->artifact_chance = value;
            else
            {
                *a_chance = value; /* no, its global for the while treasure list entry */
            }
        }
        else if (sscanf(cp, "arch %s", variable))
        {
            if ((t->item = find_archetype(variable)) == NULL)
                LOG(llevBug, "BUG: Treasure lacks archetype: %s\n", variable);
            start_marker = 1;
        }
        else if (sscanf(cp, "list %s", variable))
        {
            start_marker = 1;
            SHSTR_FREE_AND_ADD_STRING(t->name, variable);
        }
        else if (sscanf(cp, "name %s", variable))
        {
            SHSTR_FREE_AND_ADD_STRING(t->change_arch.name, cp + 5);
        }
        else if (sscanf(cp, "race %s", variable))
        {
            SHSTR_FREE_AND_ADD_STRING(t->change_arch.race, cp + 5);
        }
        else if (sscanf(cp, "title %s", variable))
        {
            SHSTR_FREE_AND_ADD_STRING(t->change_arch.title, cp + 6);
        }
        else if (sscanf(cp, "slaying %s", variable))
        {
            SHSTR_FREE_AND_ADD_STRING(t->change_arch.slaying, cp + 8);
        }
        else if (sscanf(cp, "face %s", variable))
        {
            if (strcmp (cp+5, "NONE") == 0)
            {
                t->change_arch.face_id = 0;
                t->change_arch.face = NULL;
            }
            else
            {
                int face_id = FindFace(cp+5, 0);

                if(!face_id)
                    LOG(llevBug,"BUG: TLIST can't find face %s.\n", cp+5);
                else
                {
                    t->change_arch.face_id = face_id;
                    t->change_arch.face = &new_faces[face_id];
                }
            }
        }
        else if (sscanf(cp, "anim %s", variable))
        {
            if (strcmp (cp+5, "NONE") == 0)
                t->change_arch.anim_id = 0;
            else
                t->change_arch.anim_id = find_animation (cp+5);
        }
        else if (sscanf(cp, "item_race %d", &value))
            t->change_arch.item_race = value;
        else if (sscanf(cp, "quality %d", &value))
            t->change_arch.quality = value;
        else if (sscanf(cp, "quality_range %d", &value))
            t->change_arch.quality_range = value;
        else if (sscanf(cp, "material %d", &value))
            t->change_arch.material = value;
        else if (sscanf(cp, "material_quality %d", &value))
            t->change_arch.material_quality = value;
        else if (sscanf(cp, "material_range %d", &value))
            t->change_arch.material_range = value;
        else if (sscanf(cp, "chance_fix %d", &value))
        {
            t->chance_fix = (sint16) value;
            t->chance = 0; /* important or the chance will stay 100% when not set to 0 in treasure list! */
        }
        else if (sscanf(cp, "chance %d", &value))
            t->chance = (uint8) value;
        else if (sscanf(cp, "nrof %d", &value))
            t->nrof = (uint16) value;
        else if (sscanf(cp, "magic %d", &value))
            t->magic = value;
        else if (sscanf(cp, "magic_chance %d", &value))
            t->magic_chance = value;
        else if (sscanf(cp, "difficulty %d", &value))
            t->difficulty = value;
        else if (!strncmp(cp, "yes", 3))
        {
            t_style2 = T_STYLE_UNSET;
            a_chance2 = ART_CHANCE_UNSET;
            t->next_yes = load_treasure(fp, &t_style2, &a_chance2);
            if (t->next_yes->artifact_chance == ART_CHANCE_UNSET)
                t->next_yes->artifact_chance = a_chance2;
            if (t->next_yes->t_style == T_STYLE_UNSET)
                t->next_yes->t_style = t_style2;
        }
        else if (!strncmp(cp, "no", 2))
        {
            t_style2 = T_STYLE_UNSET;
            a_chance2 = ART_CHANCE_UNSET;
            t->next_no = load_treasure(fp, &t_style2, &a_chance2);
            if (t->next_no->artifact_chance == ART_CHANCE_UNSET)
                t->next_no->artifact_chance = a_chance2;
            if (t->next_no->t_style == T_STYLE_UNSET)
                t->next_no->t_style = t_style2;
        }
        else if (!strncmp(cp, "end", 3))
            return t;
        else if (!strncmp(cp, "more", 4))
        {
            t_style2 = T_STYLE_UNSET;
            a_chance2 = ART_CHANCE_UNSET;
            t->next = load_treasure(fp, &t_style2, &a_chance2);
            if (t->next->artifact_chance == ART_CHANCE_UNSET)
                t->next->artifact_chance = a_chance2;
            if (t->next->t_style == T_STYLE_UNSET)
                t->next->t_style = t_style2;
            return t;
        }
        else
            LOG(llevBug, "BUG: Unknown treasure-command: '%s', last entry %s\n", STRING_SAFE(cp), STRING_SAFE(t->name));
    }
    LOG(llevBug, "BUG: treasure %s lacks 'end'.>%s<\n", STRING_SAFE(t->name), STRING_SAFE(cp));
    return t;
}

/*
 * Allocate and return the pointer to an empty treasurelist structure.
 */

static treasurelist_t * get_empty_treasurelist(void)
{
    treasurelist_t   *tl  = (treasurelist_t *) malloc(sizeof(treasurelist_t));
    if (tl == NULL)
        LOG(llevError, "ERROR: get_empty_treasurelist(): OOM.\n");
    tl->listname = NULL;
    tl->next = NULL;
    tl->items = NULL;
    tl->t_style = T_STYLE_UNSET; /* -2 is the "unset" marker and will virtually handled as 0 which can be overruled */
    tl->artifact_chance = ART_CHANCE_UNSET;
    tl->chance_fix = CHANCE_FIX;
    tl->total_chance = 0;
    return tl;
}

static inline void set_change_arch(_change_arch *ca)
{
    ca->face = NULL;
    ca->face_id = -1;
    ca->anim_id = -1;
    ca->animate = -1;
    ca->item_race = -1;
    ca->name = NULL;
    ca->race = NULL;
    ca->slaying = NULL;
    ca->title = NULL;
    ca->material = -1;
    ca->material_quality = -1;
    ca->material_range = -1;
    ca->quality = -1;
    ca->quality_range = -1;
}

/*
 * Allocate and return the pointer to an empty treasure structure.
 */

static treasure_t * get_empty_treasure(void)
{
    treasure_t   *t   = (treasure_t *) malloc(sizeof(treasure_t));
    if (t == NULL)
        LOG(llevError, "ERROR: get_empty_treasure(): OOM.\n");

    set_change_arch(&t->change_arch);
    t->chance_fix = CHANCE_FIX;
    t->tlist = NULL;
    t->t_style = T_STYLE_UNSET; /* -2 is the "unset" marker and will virtually handled as 0 which can be overruled */
    t->item = NULL;
    t->name = NULL;
    t->next = NULL;
    t->next_yes = NULL;
    t->next_no = NULL;
    t->artifact_chance = ART_CHANCE_UNSET;
    t->chance = 100;
    t->difficulty = 0;
    t->magic_chance = T_MAGIC_CHANCE_UNSET;
    t->magic = T_MAGIC_UNSET;
    t->nrof = 0;
    return t;
}

/*
 * Searches for the given treasurelist in the globally linked list
 * of treasurelists which has been built by load_treasures().
 */
treasurelist_t * find_treasurelist(const char *name)
{
    const char     *tmp = shstr_find(name);
    treasurelist_t   *tl;

    /* Special cases - randomitems of none is to override default.  If
    * first_treasurelist is null, it means we are on the first pass of
    * of loading archetyps, so for now, just return - second pass will
    * init these values.
    */
    if (tmp == shstr_cons.none || !first_treasurelist)
        return NULL;
    if (tmp != NULL)
    {
        for (tl = first_treasurelist; tl != NULL; tl = tl->next)
        {
            if (tmp == tl->listname)
                return tl;
        }
    }

    LOG(llevBug, "Bug: Couldn't find treasurelist %s\n", name);
    return NULL;
}

static inline treasurelist_t * find_treasurelist_intern(const char *name)
{
    treasurelist_t   *tl;

    for (tl = first_treasurelist; tl != NULL; tl = tl->next)
    {
        if (name == tl->listname)
            return tl;
    }

    return NULL;
}

/* parse treasure list paramter in randomitems cmd */
static inline void parse_tlist_parm(tlist_tweak *tweak, char *parm)
{
    char *tmp;

    do
    {
        /* the paramater list is like "parm1,parm2, parm3..." */
        if ((tmp = strchr(parm, ',')))
            *tmp = 0;

        /* every paramter is like <ID><tail> where <ID> is a single char */
        switch(*parm)
        {
            case 'm': /* (m)agic */
                tweak->magic = atoi(parm+1);
                break;
            case 'M': /* (M)agic chance */
                tweak->magic_chance = atoi(parm+1);
                break;
            case 'x': /* difficulty */
                tweak->difficulty = atoi(parm+1);
                break;
            case 'a': /* (a)rtifact  chance*/
                tweak->artifact_chance = atoi(parm+1);
                break;
            case 'd': /* (d)rop  chance 1/d */
                tweak->drop_chance = atoi(parm+1);
                break;
            case 'D': /* (D)rop  chance %  */
                tweak->drop100 = atoi(parm+1);
                break;
            case 's': /* treasure (s)tyle */
                tweak->style = atoi(parm+1);
                break;
            case 'r': /* item (r)ace */
                tweak->c_arch.item_race = atoi(parm+1);
            break;
            case 'i': /* (i)tem material */
                tweak->c_arch.material = atoi(parm+1);
                break;
            case 'I': /* (i)dentified */
                tweak->identified = 1;
                break;
            case 'c': /* (c)reated: material quality*/
                tweak->c_arch.material_quality = atoi(parm+1);
                break;
            case 'C': /* (C)reated: material range */
                tweak->c_arch.material_range = atoi(parm+1);
                break;
            case 'q': /* (q)uality */
                tweak->c_arch.quality = atoi(parm+1);
                break;
            case 'Q': /* (q)uality range */
                tweak->c_arch.quality_range = atoi(parm+1);
                break;
            case 'B': /* (B)reak list generation if this tlist generates something */
                tweak->break_list = 1;
                break;
            default:
                LOG(llevBug,"\nBUG ::TLIST PARSE: invalid tlist paramter: %s\n", STRING_SAFE(parm));
                return;
            break;
        }

        if (tmp)
            parm = tmp + 1;

    } while(tmp);
}

/* link_treasurelists will generate a linked lists of treasure list
 * using a string in format "listname1;listname2;listname3;..." as
 * argument.
 * Return: objectlink_t * to list of treasurelist
 */
objectlink_t * link_treasurelists(objectlink_t *base, char *liststring, uint32 flags)
{
    char               *tmp, *parm;
    const char         *name;
    treasurelist_t       *tl;
    objectlink_t*list =   NULL, *list_start = NULL;

    if (!first_treasurelist)
        return NULL;

    /*LOG(llevNoLog,"LINK list: %s - ",liststring);*/
    do
    {
        if ((tmp = strchr(liststring, ';')))
            *tmp = 0;

        /* find parameter marker. */
        if ((parm = strchr(liststring, '&')))
            *parm = 0;

        if (!(name = shstr_find(liststring)))
        {
            /* no treasure list name in hash table = no treasure list with that name */
            LOG(llevInfo, "BUG: link_treasurelists(): Treasurelist >%s< not found\n", liststring);
        }
        else
        {
            /* we have a 'none' string?
             * normally, we should break here but perhaps
             * we have something like "list1;none;list2".
             * Can't think why but lets do it right.
             */
            if (name != shstr_cons.none)
            {
                tl = find_treasurelist_intern(name);
                if (!tl)
                    LOG(llevInfo, "BUG: link_treasurelists(): Treasurelist >%s< not found\n", liststring);
                else
                {
                    if (list)
                    {
                        list->next = objectlink_get(OBJLNK_FLAG_TL);
                        list->next->prev = list;
                        list = list->next;
                    }
                    else
                    {
                        list_start = list = objectlink_get(OBJLNK_FLAG_TL);
                        /* important: we only mark the first list node with
                                         * static or refcount flag.
                                         */
                        list->flags |= flags;
                        if (flags & OBJLNK_FLAG_REF)
                            list->ref_count++;
                        /*LOG(llevNoLog," --> %x (%d)\n",list->flags,list->ref_count);*/
                    }

                    list->objlink.tl = tl;

                    if(parm) /* we have a parameter list ('&' tail) for this list? parse it */
                    {
                        tlist_tweak  *tweak = (tlist_tweak *) get_poolchunk(pool_tlist_tweak);

                        /* save the tname with paramter. don't patch, save the whole name */
                        *parm = '&';
                        tweak->name = shstr_add_string(liststring);
                        tweak->artifact_chance = ART_CHANCE_UNSET;
                        tweak->style = T_STYLE_UNSET;
                        tweak->difficulty = 0;
                        tweak->identified = 0;
                        tweak->break_list = 0;
                        tweak->magic = T_MAGIC_UNSET;
                        tweak->magic_chance = T_MAGIC_CHANCE_UNSET;
                        tweak->drop_chance = 0;
                        tweak->drop100 = 0;
                        set_change_arch(&tweak->c_arch);
                        parse_tlist_parm(tweak, parm+1);
                        list->parmlink.tl_tweak = tweak;
                    }
                }
            }
        }
        if (tmp)
            liststring = tmp + 1;
    }
    while (tmp);

    if (base)
    {
        list->next = base;
        base->prev = list;
    }

    return list_start;
}

objectlink_t *branch_treasurelist(objectlink_t *tl)
{
    objectlink_t *list = NULL,
                 *list_start = NULL,
                 *new;
    tlist_tweak *tweak = NULL;

    for (list = tl; list; list = list->next)
    {
        if (!list_start)
        {
            new = list_start = objectlink_get(OBJLNK_FLAG_TL);
            new->flags |= OBJLNK_FLAG_STATIC;
            new->ref_count++;
        }
        else
        {
            new->next = objectlink_get(OBJLNK_FLAG_TL);
            new->next->prev = new;
            new = new->next;
        }

        new->objlink = list->objlink;

        if (list->parmlink.tl_tweak)
        {
            new->parmlink.tl_tweak = (tlist_tweak *)get_poolchunk(pool_tlist_tweak);
            memcpy(&(new->parmlink), &(list->parmlink), sizeof(list->parmlink));
            new->parmlink.tl_tweak->name = shstr_add_string(list->parmlink.tl_tweak->name);
        }
    }

    return list_start;
}

void append_treasure_string(const object_t *op, char *buf)
{
    if (!op->randomitems)
    {
        strcat(buf, "none\n");
    }
    else /* we have different lists - generate the "list1;list2;list3" string */
    {
        objectlink_t *ol = op->randomitems;
        if (ol->parmlink.tl_tweak) /* tlist with or without '&' parameter list* */
        {
            strcat(buf, ol->parmlink.tl_tweak->name); /* save name and '&' tail */
        }
        else
        {
            strcat(buf, ol->objlink.tl->listname); /* clean default list name */
        }

        ol = ol->next;

        for (; ol; ol = ol->next)
        {
            strcat(buf, ";");
            if (ol->parmlink.tl_tweak)
            {
                strcat(buf, ol->parmlink.tl_tweak->name);
            }
            else
            {
                strcat(buf, ol->objlink.tl->listname);
            }
        }
    }
}

/* unlink a treasure list.
 * if flag is set to 1, ignore (delete) the OBJLNK_FLAG_STATIC flag
 */
void unlink_treasurelists(objectlink_t *list, int flag)
{
    /*LOG(llevNoLog,"unlink list: %s (%x - %d)\n",list->objlink.tl->listname, list->flags, list->ref_count );*/
    if (list && (list->flags & OBJLNK_FLAG_REF))
        list->ref_count--;

    /* skip if we have no list or refcount shows
     * that still other objects points to this list.
     */
    if (!list || list->ref_count || ((list->flags & OBJLNK_FLAG_STATIC) && !flag))
    {
        /*LOG(llevNoLog,"skiped listpart: %s\n",list->objlink.tl->listname);*/
        return;
    }

    do
    {
        /*LOG(llevNoLog,"freed listpat: %s\n",list->objlink.tl->listname); */
        if(list->parmlink.tl_tweak)
        {
            SHSTR_FREE(list->parmlink.tl_tweak->name);
            return_poolchunk(list->parmlink.tl_tweak, pool_tlist_tweak);
        }
        return_poolchunk(list, pool_objectlink);
        /* hm, this should work... return_poolchunk() should not effect the objectlink itself */
        list = list->next;
    }
    while (list);
}

/* The point of generate_treasure is that is generates exactly *one*
 * item. I tweaked it a bit so it will work now with a linked list.
 * generate_treasure can give back NULL (nothing generated).
 * We will browse the linked list until we get a valid target.
 * We break if we have a item or we are at the end of the list.
 * MT-2005
 */
object_t * generate_treasure(struct objectlink_t *t, int difficulty)
{
    struct _change_arch *captr;
    int t_style, a_chance, flag, magic, magic_chance;
    object_t*ob =     get_object(), *tmp = NULL;

    while (t)
    {
        flag = 0;
        captr = NULL;
        t_style = t->objlink.tl->t_style;
        a_chance = t->objlink.tl->artifact_chance;
        magic = T_MAGIC_UNSET;
        magic_chance = T_MAGIC_CHANCE_UNSET;

        if(t->parmlink.tl_tweak) /* this treasure list had a '&' paramter list */
        {
            /* random chance tests for 1/x, drop100 for % chance (1-100%) */
            if((t->parmlink.tl_tweak->drop_chance && (RANDOM() % t->parmlink.tl_tweak->drop_chance))
                 || (t->parmlink.tl_tweak->drop100 && ((RANDOM()%100) >= t->parmlink.tl_tweak->drop100)))
            {
                t = t->next;
                continue;
            }

            /* setup the '&' parameter values insertation to the treasure list */
            captr = &t->parmlink.tl_tweak->c_arch;
            magic = t->parmlink.tl_tweak->magic;
            magic_chance = t->parmlink.tl_tweak->magic_chance;
            if(t->parmlink.tl_tweak->style != T_STYLE_UNSET)
                t_style = t->parmlink.tl_tweak->style;
            if(t->parmlink.tl_tweak->difficulty)
                difficulty = t->parmlink.tl_tweak->difficulty;
            if(t->parmlink.tl_tweak->artifact_chance != ART_CHANCE_UNSET)
                a_chance = t->parmlink.tl_tweak->artifact_chance;
            if(t->parmlink.tl_tweak->identified)
                flag |= GT_IDENTIFIED;
        }

        create_treasure(t->objlink.tl, ob, flag, difficulty, t_style, a_chance, magic, magic_chance, 0, captr);

        if (!ob->inv) /* no treasure, try next tlist */
        {
            t = t->next;
            continue;
        }

        /* Don't want to free the object we are about to return */
        tmp = ob->inv;
        remove_ob(tmp); /* remove from inv - no move off */
        if (ob->inv)
        {
            LOG(llevBug, "BUG: generate treasure(): created multiple objects for tlist %s.\n",
                STRING_SAFE(t->objlink.tl->listname));
            /* remove objects so garbage collection can put them back */
            for (; ob->inv;)
                remove_ob(ob->inv);
        }
        return tmp;
    }
    return tmp;
}

/* This calls the appropriate treasure creation function.  tries is passed
 * to determine how many list transitions or attempts to create treasure
 * have been made.  It is really in place to prevent infinite loops with
 * list transitions, or so that excessively good treasure will not be
 * created on weak maps, because it will exceed the number of allowed tries
 * to do that.
 */
/* called from various sources, arch_change will be normally NULL the first time a
 * treasure list is generated. This is then the "base" list. We will use the real
 * first arch_change as base to other recursive calls.
 */
/* help function to call a objectlink linked list of treasure lists */
void create_treasure_list(struct objectlink_t *t, object_t *op, int flag, int difficulty, int art_chance, int tries)
{
    struct _change_arch *captr;
    int t_style, a_chance, magic, magic_chance;

    while (t)
    {
        if(t->parmlink.tl_tweak) /* this treasure list had a '&' paramter list */
        {
            /* random chance tests for 1/x, drop100 for % chance (1-100%) */
            if((t->parmlink.tl_tweak->drop_chance && (RANDOM() % t->parmlink.tl_tweak->drop_chance))
                || (t->parmlink.tl_tweak->drop100 && ((RANDOM()%100) >= t->parmlink.tl_tweak->drop100)))
            {
                t = t->next;
                continue;
            }

            a_chance = art_chance;
            /* setup the '&' parameter values to the treasure list */
            captr = &t->parmlink.tl_tweak->c_arch;

            magic = t->parmlink.tl_tweak->magic;
            magic_chance = t->parmlink.tl_tweak->magic_chance;

            if(t->parmlink.tl_tweak->style != T_STYLE_UNSET)
                t_style = t->parmlink.tl_tweak->style;
            else
                t_style = t->objlink.tl->t_style;

            if(t->parmlink.tl_tweak->artifact_chance != ART_CHANCE_UNSET)
                a_chance = t->parmlink.tl_tweak->artifact_chance;
            else if(t->objlink.tl->artifact_chance != ART_CHANCE_UNSET)
                a_chance = t->objlink.tl->artifact_chance;

            if(t->parmlink.tl_tweak->difficulty)
                difficulty = t->parmlink.tl_tweak->difficulty;
            if(t->parmlink.tl_tweak->identified)
                flag |= GT_IDENTIFIED;
        }
        else
        {
            a_chance = art_chance;
            if(t->objlink.tl->artifact_chance != ART_CHANCE_UNSET)
                a_chance = t->objlink.tl->artifact_chance;
            captr = NULL;
            t_style = T_STYLE_UNSET;
            magic = T_MAGIC_UNSET;
            magic_chance = T_MAGIC_CHANCE_UNSET;

        }
        /* if we have a breakpoint set here, stop creating treasures for this list */
        if( create_treasure(t->objlink.tl, op, flag, difficulty, t_style, a_chance, magic, magic_chance, tries, captr) &&
                (t->parmlink.tl_tweak && t->parmlink.tl_tweak->break_list))
            return;

        t = t->next;
    }
}

int create_treasure(treasurelist_t *t, object_t *op, int flag, int difficulty, int t_style, int a_chance,
        int magic, int magic_chance, int tries, struct _change_arch *arch_change)
{
    int ret = 0;

    if (tries++ > 100)
    {
        LOG(llevDebug, "create_treasure(): tries >100 for t-list %s.", t->listname ? t->listname : "<noname>");
        return ret;
    }

    if (t->t_style != T_STYLE_UNSET)
        t_style = t->t_style;
    if (t->artifact_chance != ART_CHANCE_UNSET)
        a_chance = t->artifact_chance;

    if (t->total_chance)
        ret = create_one_treasure(t, op, flag, difficulty, t_style, a_chance, magic, magic_chance, tries, arch_change);
    else
        ret = create_all_treasures(t->items, op, flag, difficulty, t_style, a_chance, magic, magic_chance,tries, arch_change);

    return ret;
}

/*
 * Generates the objects specified by the given treasure.
 * It goes recursively through the rest of the linked list.
 * If there is a certain percental chance for a treasure to be generated,
 * this is taken into consideration.
 * The second argument specifies for which object the treasure is
 * being generated.
 * If flag is GT_INVISIBLE, only invisible objects are generated (ie, only
 * abilities.  This is used by summon spells, thus no summoned monsters
 * start with equipment, but only their abilities).
 */
int create_all_treasures(treasure_t *t, object_t *op, int flag, int difficulty, int t_style, int a_chance,
                          int magic, int magic_chance, int tries, struct _change_arch *change_arch)
{
    int     ret = 0;
    object_t *tmp;

    /*  LOG(llevNoLog,"-CAT-: %s (%d)\n", STRING_SAFE(t->name),change_arch?t->change_arch.material_quality:9999); */
    /* LOG(llevNoLog,"CAT: cs: %d (%d)(%s)\n", t->chance_fix, t->chance, t->name); */
    if (t->t_style != T_STYLE_UNSET)
        t_style = t->t_style;
    if (t->artifact_chance != ART_CHANCE_UNSET)
        a_chance = t->artifact_chance;

    if ((t->chance_fix != CHANCE_FIX && !(RANDOM() % (int) t->chance_fix))
     || (int) t->chance >= 100
     || ((RANDOM() % 100 + 1) < (int) t->chance))
    {
        /*LOG(llevNoLog,"CAT22: cs: %d (%d)(%s)\n", t->chance_fix, t->chance, t->name);*/
        if (t->tlist && difficulty >= t->difficulty)
        {
            /*  LOG(llevNoLog,"-CAT2: %s (%d)\n", STRING_SAFE(t->name),change_arch?t->change_arch.material_quality:9999); */
            ret = create_treasure(t->tlist, op, flag, difficulty, t_style, a_chance,
                t->magic!=T_MAGIC_UNSET?magic:t->magic,
                t->magic_chance!=T_MAGIC_CHANCE_UNSET?magic_chance:t->magic_chance,
                tries, change_arch ? change_arch : &t->change_arch);
        }
        else if (t->item && t->item->name != shstr_cons.none && difficulty >= t->difficulty)
        {
            if (QUERY_FLAG(&t->item->clone, FLAG_SYS_OBJECT) ||
                !(flag & GT_INVISIBLE))
            {
                ret = 1; /* we have generated an item! */
                if (t->item->clone.type != TYPE_WEALTH)
                {
                    /*LOG(llevNoLog,"*CAT*: %s (%d)\n", t->item->clone.name,change_arch?t->change_arch.material_quality:9999); */
                    tmp = arch_to_object(t->item);
                    if (t->nrof && tmp->nrof <= 1)
                        tmp->nrof = RANDOM() % ((int) t->nrof) + 1;

                    set_material_real(tmp, &t->change_arch);
                    change_treasure(&t->change_arch, tmp);
                    if(change_arch)
                    {
                        set_material_real(tmp, change_arch);
                        change_treasure(change_arch, tmp);
                    }
                    fix_generated_item(&tmp, op, difficulty, a_chance, t_style,
                            (t->magic==T_MAGIC_UNSET)?magic:t->magic,
                            (t->magic_chance==T_MAGIC_CHANCE_UNSET)?magic_chance:t->magic_chance, flag);
                    PUT(tmp, op, flag);
                    /* if treasure is "identified", created items are too */
                    if (op->type == TREASURE && QUERY_FLAG(op, FLAG_IDENTIFIED))
                    {
                        THING_IDENTIFY(tmp);
                    }
                }
                else /* we have a wealth object - expand it to real money */
                {
                    /* if t->magic is != 0, thats our value - if not use default setting */
                    int i;
                    sint64 value = t->magic==T_MAGIC_UNSET?magic==T_MAGIC_UNSET?t->item->clone.value:magic:t->magic;

                    value *= (difficulty / 2) + 1;
                    /* so we have 80% to 120% of the fixed value */
                    value = (sint64) ((float) value * 0.8f + (float) value * ((float) (RANDOM() % 40) / 100.0f));
                    for (i = 0; i < NUM_COINS; i++)
                    {
                        if (value / coins_arch[i]->clone.value > 0)
                        {
                            tmp = get_object();
                            copy_object(&coins_arch[i]->clone, tmp);
                            tmp->nrof = (uint32) (value / tmp->value);
                            value -= tmp->nrof * tmp->value;
                            PUT(tmp, op, flag);
                        }
                    }
                }
            }
        }

        if (t->next_yes != NULL)
            ret = create_all_treasures(t->next_yes, op, flag, difficulty,
                                 (t->next_yes->t_style == T_STYLE_UNSET) ? t_style : t->next_yes->t_style, a_chance,
                                 (t->magic!=T_MAGIC_UNSET)?t->next_yes->magic:magic,
                                 (t->magic_chance!=T_MAGIC_CHANCE_UNSET)?t->next_yes->magic_chance:magic,
                                 tries, change_arch);
    }
    else if (t->next_no != NULL)
        ret = create_all_treasures(t->next_no, op, flag, difficulty,
                             (t->next_no->t_style == T_STYLE_UNSET) ? t_style : t->next_no->t_style, a_chance,
                             (t->magic!=T_MAGIC_UNSET)?t->next_no->magic:magic,
                             (t->magic_chance!=T_MAGIC_CHANCE_UNSET)?t->next_no->magic_chance:magic,
                             tries, change_arch);
    if (t->next != NULL)
        ret = create_all_treasures(t->next, op, flag, difficulty,
                             (t->next->t_style == T_STYLE_UNSET) ? t_style : t->next->t_style, a_chance,
                             (t->magic!=T_MAGIC_UNSET)?t->next->magic:magic,
                             (t->magic_chance!=T_MAGIC_CHANCE_UNSET)?t->next->magic_chance:magic,
                             tries, change_arch);
    return ret;
}

// this called by create_treasure,
// which is nested in create_all_treasures and can also call create_all_treasures
// where we can also call create_treasures again
// so very deep recursion over 3 functions
int create_one_treasure(treasurelist_t *tl, object_t *op, int flag, int difficulty, int t_style, int a_chance,
                         int magic, int magic_chance,int tries,struct _change_arch *change_arch)
{
    int         ret = 0, value, diff_tries = 0;
    treasure_t   *t;
    object_t     *tmp;

    /*LOG(llevNoLog,"-COT-: %s (%d)\n", tl->name,change_arch?tl->items->change_arch.material_quality:9999); */
    /*LOG(llevNoLog,"COT: cs: %d (%s)\n", tl->chance_fix, tl->name );*/
    if (tries++ > 100)
        return ret;

    /* well, at some point we should rework this whole system... */
    create_one_treasure_again_jmp:
    if (diff_tries > 10)
        return ret;
    value = RANDOM() % tl->total_chance;

    // very strange logic to roll against chance and difficulty,
    // till we found something or reached the limit of allowed rolls
    // we loop items in list, this means first items have always advantages to be picked?
    for (t = tl->items; t != NULL; t = t->next)
    {
        /* chance_fix will overrule the normal chance stuff! */
        if (t->chance_fix != CHANCE_FIX)
        {
            if (!(RANDOM() % t->chance_fix))
            {
                /* LOG(llevNoLog,"COT: HIT: cs: %d (%s)\n", t->chance_fix, t->name);*/
                /* only when allowed, we go on! */
                if (difficulty >= t->difficulty)
                {
                    value = 0;
                    break;
                }

                /* ok, difficulty is bad lets try again or break! */
                if (tries++ > 100)
                    return ret;
                diff_tries++;
                goto create_one_treasure_again_jmp;
            }

            if (!t->chance)
                continue;
        }
        value -= t->chance;
        if (value <= 0) /* we got one! */
        {
            /* only when allowed, we go on! */
            if (difficulty >= t->difficulty)
                break;

            /* ok, difficulty is bad lets try again or break! */
            if (tries++ > 100)
                return ret;
            diff_tries++;
            goto create_one_treasure_again_jmp;
        }
    }

    if (t->t_style != T_STYLE_UNSET)
        t_style = t->t_style;
    if (t->artifact_chance != ART_CHANCE_UNSET)
        a_chance = t->artifact_chance;

    if (!t || value > 0)
    {
        LOG(llevBug, "BUG: create_one_treasure: got null object or not able to find treasure - tl:%s op:%s\n",
            PTR_STRING_SAFE(tl, listname), STRING_OBJ_NAME(op));
        return ret;
    }

    // if we picked a list, we need to start next recursion
    if (t->tlist)
    {
        if (difficulty >= t->difficulty)
            ret = create_treasure(t->tlist, op, flag, difficulty, t_style, a_chance,
                            t->magic==T_MAGIC_UNSET?magic:t->magic,
                            t->magic_chance==T_MAGIC_CHANCE_UNSET?magic_chance:t->magic_chance,
                            tries, change_arch);
        else if (t->nrof)
            ret = create_one_treasure(tl, op, flag, difficulty, t_style, a_chance,
                                t->magic==T_MAGIC_UNSET?magic:t->magic,
                                t->magic_chance==T_MAGIC_CHANCE_UNSET?magic_chance:t->magic_chance,
                                tries, change_arch);
        return ret;
    }

    // finally we can handle one item here

    // sanity checks
    if (t->item &&
        t->item->name != shstr_cons.none &&
        (QUERY_FLAG(&t->item->clone, FLAG_SYS_OBJECT) ||
         flag != GT_INVISIBLE))
    {
        ret = 1;
        if (t->item->clone.type != TYPE_WEALTH)
        {
            /*LOG(llevNoLog,"*COT*: %s (%d)\n", t->item->clone.name,change_arch?t->change_arch.material_quality:9999); */

            // finally we pick this arch
            tmp = arch_to_object(t->item);
            // when arches have no nrof definition
            // roll 1-x nrof in relation to treasurelist definition
            if (t->nrof && tmp->nrof <= 1)
                tmp->nrof = RANDOM() % ((int) t->nrof) + 1;

            // here we call set_material_real, where tmp is our arch definition and t is treasurelist
            set_material_real(tmp, &t->change_arch);

            change_treasure(&t->change_arch, tmp);
            // WHY we call this again? This is pointer shit
            if(change_arch)
            {
                set_material_real(tmp, change_arch);
                change_treasure(change_arch, tmp);
            }
            // !this is where we do balance changes like setting condition to 100
            // but what happens to non items, when we remove condition setting from set_material

            fix_generated_item(&tmp, op, difficulty, a_chance,
                                    (t->t_style == T_STYLE_UNSET) ? t_style : t->t_style,(t->magic!=T_MAGIC_UNSET)?t->magic:magic,
                                    (t->magic_chance!=T_MAGIC_CHANCE_UNSET)?t->magic_chance:magic_chance, flag);
            PUT(tmp, op, flag);
            /* if treasure is "identified", created items are too */
            if (op->type == TREASURE && QUERY_FLAG(op, FLAG_IDENTIFIED))
            {
                THING_IDENTIFY(tmp);
            }
        }
        else /* we have a wealth object - expand it to real money */
        {
            /* if t->magic is != 0, thats our value - if not use default setting */
            int i;
            sint64 value = t->magic==T_MAGIC_UNSET?magic==T_MAGIC_UNSET?t->item->clone.value:magic:t->magic;

            value *= difficulty;
            /* so we have 80% to 120% of the fixed value */
            value = (sint64) ((float) value * 0.8f + (float) value * ((float) (RANDOM() % 40) / 100.0f));
            for (i = 0; i < NUM_COINS; i++)
            {
                if (value / coins_arch[i]->clone.value > 0)
                {
                    tmp = get_object();
                    copy_object(&coins_arch[i]->clone, tmp);
                    tmp->nrof = (uint32)(value / tmp->value);
                    value -= tmp->nrof * tmp->value;
                    PUT(tmp, op, flag);
                }
            }
        }
    }
    return ret;
}

/* if there are change_xxx commands in the treasure, we include the changes
 * in the generated object
 */
static void change_treasure(struct _change_arch *ca, object_t *op)
{
    if(ca->face_id != -1)
        op->face = ca->face;

    if(ca->anim_id != -1)
        op->animation_id = ca->anim_id;

    if(ca->animate != -1)
    {
        if(ca->animate)
            SET_FLAG(op, FLAG_ANIMATE);
        else
            CLEAR_FLAG(op, FLAG_ANIMATE);
    }

    if (ca->name)
    {
        SHSTR_FREE_AND_ADD_STRING(op->name, ca->name);
    }

    if (ca->race)
    {
        SHSTR_FREE_AND_ADD_STRING(op->race, ca->race);
    }

    if (ca->title)
    {
        SHSTR_FREE_AND_ADD_STRING(op->title, ca->title);
    }

    if (ca->slaying)
    {
        SHSTR_FREE_AND_ADD_STRING(op->slaying, ca->slaying);
    }
}

/*
 * Sets a random magical bonus in the given object based upon
 * the given difficulty, and the given max possible bonus.
 * the old system based on difficulty for setting the +x value.
 * i changed this in 2 ways: difficulty now effects only artifacts
 * but now artifacts generation don't based on +x value anymore.
 * so, a sword+1 can be Sunword +3 if difficulty is right.
 * The reason why we don't want +3 or +4 "base" items is that
 * we want include crafting later - there we can build from 2 +1 items
 * of same kind one +2.
 */
static void set_magic(int difficulty, object_t *op, int max_magic, int fix_magic, int chance_magic, int flags)
{
    int i;

    if (fix_magic) /* if we have a fixed value, force it */
        i = fix_magic;
    else
    {
        i = 0;

        if (((RANDOM() % 100) + 1) <= chance_magic) /* chance_magic 0 means allways no magic bonus */
        {
            i = (RANDOM() % abs(max_magic)) + 1;
            if (max_magic < 0)
                i = -i;
        }
    }


    if ((flags & GT_ONLY_GOOD) && i < 0)
        i = 0;
    set_abs_magic(op, i);

    if (i < 0)
        SET_FLAG(op, FLAG_CURSED);
    if (i != 0)
        SET_FLAG(op, FLAG_IS_MAGICAL);
}


/*
 * Sets magical bonus in an object, and recalculates the effect on
 * the armour variable, and the effect on speed of armour.
 * This function doesn't work properly, should add use of archetypes
 * to make it truly absolute.
 */
void set_abs_magic(object_t *op, int magic)
{
    if (!magic)
        return;


    SET_FLAG(op, FLAG_IS_MAGICAL);

    op->magic = magic;
    if (op->arch)
    {
/*      if (magic == 1)
           op->value += 5300;
       else if (magic == 2)
           op->value += 12300;
       else if (magic == 3)
           op->value += 62300;
       else if (magic == 4)
           op->value += 130300;
       else
           op->value += 250300; 13 Jul 07 L00natyk & Longir*/
    /*    here can be added additive system too and we can switch it by
    eg. is_multiplicative 0/1 so it will make items pricing more
    elastic. 13 Jul 07 L00natyk & Longir
    */
        op->value += (op->value * magic);

        if (op->type == ARMOUR)
            op->last_sp = (op->arch->clone.last_sp * (100 + magic * 10)) / 100;

        if (magic < 0 && !(RANDOM() % 3)) /* You can't just check the weight always */
            magic = (-magic);
        op->weight = (op->arch->clone.weight * (100 - magic * 10)) / 100;
    }
    else
    {
        if (op->type == ARMOUR)
            op->last_sp = (op->last_sp * (100 + magic * 10)) / 100;
        if (magic < 0 && !(RANDOM() % 3)) /* You can't just check the weight always */
            magic = (-magic);
        op->weight = (op->weight * (100 - magic * 10)) / 100;
    }
}

int roll_ring_bonus_stat(int current, int level, int bonus) {

     int roll;

     if(level <= 20)
     {
          roll = 1;
     } else if(level <= 50)
     {
          roll = RANDOM_ROLL(1,2);
     } else if(level <= 80)
     {
          if (current == 6)
          {
               roll = RANDOM_ROLL(1,2);
          } else
          {
               roll = RANDOM_ROLL(1,3);
          }
     } else if(level <= 100)
     {
          if (current >= 7)
          {
               roll = RANDOM_ROLL(1,2);
          } else
          {
               roll = RANDOM_ROLL(1,4);
          }
     } else if(level > 100)
     {
          if (current >= 7)
          {
               roll = RANDOM_ROLL(1,2);
          } else
          {
               roll = RANDOM_ROLL(1,5);
          }
     }
     if (bonus < 0)
     {
          return (roll * -1);
     } else
     {
          return roll;
     }
}

void set_ring_bonus_value_calc(object_t *op)
{
    /*    here we can easy adjust values for each stat and to annoy
    players cursed rings will be in material price as stats
    does nothing. :D
    */
    //op->value = 0;
    int resist;

    for(resist = 0; resist < num_resist_table; resist++) {
    if (op->resist[resist_table[resist]] > 0) {
        op->value += (sint64)(op->resist[resist_table[resist]] * (op->item_quality / 100.0f) * 500);
        }
    }

    if (op->stats.Str > 0) op->value += op->stats.Str * 10400;
    if (op->stats.Dex > 0) op->value += op->stats.Dex * 10030;
    if (op->stats.Con > 0) op->value += op->stats.Con * 10002;
    if (op->stats.Int > 0) op->value += op->stats.Int * 10004;
    if (op->stats.Wis > 0) op->value += op->stats.Wis * 10005;
    if (op->stats.Pow > 0) op->value += op->stats.Pow * 10010;
    if (op->stats.Cha > 0) op->value += op->stats.Cha * 10001;

    if (op->stats.wc > 0) op->value += op->stats.wc * 10010;
    if (op->stats.ac > 0) op->value += op->stats.ac * 10040;

    if (op->stats.dam > 0) op->value += op->stats.dam * 5003;

    if (op->stats.sp > 0) op->value += op->stats.sp * 11002;
    if (op->stats.hp > 0) op->value += op->stats.hp * 11001;

    if (op->stats.maxsp > 0) op->value += op->stats.maxsp * 603;
    if (op->stats.maxhp > 0) op->value += op->stats.maxhp * 501;

    if (op->stats.exp > 0) op->value += op->stats.exp * 3002;

    if(op->type == AMULET) {
        if(QUERY_FLAG(op, FLAG_REFL_MISSILE)) op->value += (sint64)(20000 * (op->item_quality / 100.0f));
        if(QUERY_FLAG(op, FLAG_REFL_CASTABLE)) op->value += (sint64)(60000 * (op->item_quality / 100.0f));
    }

    /* freak copper coins for make prices more fun */

    op->value += RANDOM_ROLL(0, 199);
}
/*
 * Randomly adds one magical ability to the given object.
 * Modified for Partial Resistance in many ways:
 * 1) Since rings can have multiple bonuses, if the same bonus
 *  is rolled again, increase it - the bonuses now stack with
 *  other bonuses previously rolled and ones the item might natively have.
 * 2) Add code to deal with new PR method.
 * return 0: no special added. 1: something added.
 */
int set_ring_bonus(object_t *op, int bonus, int level)
{
/*    int tmp, r, off;*/
    int tmp, off, roll, r;
    off = (level >= 50 ? 1 : 0) + (level >= 60 ? 1 : 0) + (level >= 70 ? 1 : 0) + (level >= 80 ? 1 : 0);
    set_ring_bonus_jump1: /* lets repeat, to lazy for a loop */
/*    r = RANDOM() % (bonus > 0 ? 15 : 13);
    SET_FLAG(op, FLAG_IS_MAGICAL);*/

    if(op->type == AMULET) {
/*     if (!(RANDOM() % 21))
           r = 20 + RANDOM() % 2;
       else if (!(RANDOM() % 20))
       {
           tmp = RANDOM() % 3;
           if (tmp == 2)
               r = 0;
           else if (!tmp)
               r = 11;
           else
               r = 12;
       }
       else if (RANDOM() & 2)
           r = 10;
       else
           r = 13 + RANDOM() % 7;*/
    roll = RANDOM_ROLL(0,100);

    if(roll < 7) {
        r = 0; /* hp + */
        }
    else if(roll < 15) {
        r = 1; /* sp + */
        }
    else if(roll < 70) {
        r = 2; /* resist */
        }
    else if(roll < 75) {
        r = 3; /* reflect spells */
        }
    else if(roll < 80) {
        r = 4; /* reflect missles */
        }
    else {
        r = 14; /* ac */
        }
    }
/*   switch (r % 25)*/
    else {
    roll = RANDOM_ROLL(0,100);

    if(roll < 4) {
        r = 0; /* hp + */
        }
    else if(roll < 9) {
        r = 1; /* sp + */
        }
    else if(roll < 29) {
        r = 2; /* resist */
        }
    else if(roll < 34) {
        r = 3; /* sp regen */
        }
    else if(roll < 39) {
        r = 4; /* hp regen */
        }
    else if(roll < 40) {
        r = 5; /* str */
        }
    else if(roll < 42) {
        r = 6; /* dex */
        }
    else if(roll < 44) {
        r = 7; /* con */
        }
    else if(roll < 46) {
        r = 8; /* int */
        }
    else if(roll < 48) {
        r = 9; /* wis */
        }
    else if(roll < 50) {
        r = 10; /* pow */
        }
    else if(roll < 52) {
        r = 11; /* cha */
        }
    else if(roll < 58) {
        r = 12; /* dam */
        }
    else if(roll < 65) {
        r = 13; /* wc */
        }
    else if(roll < 75) {
        r = 14; /* ac */
        }
    else {
        r = 15; /* speed */
        }

    }

    switch (r)
    {
          /* Redone by MSW 2000-11-26 to have much less code.  Also,
           * bonuses and penalties will stack and add to existing values.
           * of the item.
           */
        case 0:
          /* we are creating hp stuff! */
          tmp = 5;
          if (level < 5)
          {
              tmp += RANDOM() % 10;
          }
          else if (level < 10)
          {
              tmp += 10 + RANDOM() % 10;
          }
          else if (level < 15)
          {
              tmp += 15 + RANDOM() % 20;
          }
          else if (level < 20)
          {
              tmp += 20 + RANDOM() % 21;
          }
          else if (level < 25)
          {
              tmp += 25 + RANDOM() % 23;
          }
          else if (level < 30)
          {
              tmp += 30 + RANDOM() % 25;
          }
          else if (level < 40)
          {
              tmp += 40 + RANDOM() % 30;
          }
          else
          {
              tmp += (int) ((double) level * 0.65) + 50 + RANDOM() % 40;
          }
          if (bonus < 0)
          {
              tmp = -tmp;
          }
          else
          {
              op->item_level = (int) ((double) level * (0.5 + ((double) (RANDOM() % 40) / 100.0)));
          }
          op->stats.maxhp = tmp;
          break;
        case 1:
    // mana+
          tmp = 3;
          if (level < 5)
          {
              tmp += RANDOM() % 3;
          }
          else if (level < 10)
          {
              tmp += 3 + RANDOM() % 4;
          }
          else if (level < 15)
          {
              tmp += 4 + RANDOM() % 6;
          }
          else if (level < 20)
          {
              tmp += 6 + RANDOM() % 8;
          }
          else if (level < 25)
          {
              tmp += 8 + RANDOM() % 10;
          }
          else if (level < 33)
          {
              tmp += 10 + RANDOM() % 12;
          }
          else if (level < 44)
          {
              tmp += 15 + RANDOM() % 15;
          }
          else
          {
              tmp += (int) ((double) level * 0.53) + 20 + RANDOM() % 20;
          }
          if (bonus < 0)
          {
              tmp = -tmp;
          }
          else
          {
              op->item_level = (int) ((double) level * (0.5 + ((double) (RANDOM() % 40) / 100.0)));
          }
          op->stats.maxsp = tmp;
          break;
        case 2:
        {
        //resist
              int b = 5 + ABS( bonus),val,resist = RANDOM() % (num_resist_table - 4 + off);

              /* Roughly generate a bonus between 100 and 35 (depending on the bonus) */
              val = 10 + RANDOM() % b + RANDOM() % b + RANDOM() % b + RANDOM() % b;

              /* Cursed items need to have higher negative values to equal out with
                 * positive values for how protections work out.  Put another
                 * little random element in since that they don't always end up with
                 * even values.
                 */
              if (bonus < 0)
                  val = 2 * -val - RANDOM() % b;

              if (val > 35)
                  val = 35; /* Upper limit */
              b = 0;
              while (op->resist[resist_table[resist]] != 0)
              {
                  if (b++ >= 4)
                      goto set_ring_bonus_jump1; /* Not able to find a free resistance */
                  resist = RANDOM() % (num_resist_table - 4 + off);
              }
              op->resist[resist_table[resist]] = val;
              /* We should probably do something more clever here to adjust value
                 * based on how good a resistance we gave.
                 */
              break;
          }
        case 3:
          if (op->type == AMULET)
          {
              SET_FLAG(op, FLAG_REFL_CASTABLE);
          if(bonus < 0) {
            op->item_quality -= RANDOM_ROLL(1,30), op->item_condition = 100;
        }
          }
          else
          {
        if(bonus > 0) {
        roll = RANDOM_ROLL(1,10);
        op->stats.sp = roll; /* regenerate spell points */
          }
         else {
        roll = RANDOM_ROLL(1,3);
        op->stats.sp = (roll * -1);
        }
          }
          break;

        case 4:
          if (op->type == AMULET)
          {
              SET_FLAG(op, FLAG_REFL_MISSILE);
          if(bonus < 0) {
            op->item_quality -= RANDOM_ROLL(1,30), op->item_condition = 100;
        }
          }
          else
          {
        if(bonus > 0) {
        roll = RANDOM_ROLL(1,10);
        op->stats.hp = roll; /* regenerate hit points */
          }
         else {
        roll = RANDOM_ROLL(1,3);
        op->stats.hp = (roll * -1);
        }
          }
          break;
        case 5:
        op->stats.Str += roll_ring_bonus_stat(op->stats.Str, level, bonus);
        break;
        case 6:
        op->stats.Dex += roll_ring_bonus_stat(op->stats.Dex, level, bonus);
        break;
        case 7:
        op->stats.Con += roll_ring_bonus_stat(op->stats.Con, level, bonus);
        break;
        case 8:
        op->stats.Int += roll_ring_bonus_stat(op->stats.Int, level, bonus);
        break;
        case 9:
        op->stats.Wis += roll_ring_bonus_stat(op->stats.Wis, level, bonus);
        break;
        case 10:
        op->stats.Pow += roll_ring_bonus_stat(op->stats.Pow, level, bonus);
        break;
        case 11:
        op->stats.Cha += roll_ring_bonus_stat(op->stats.Cha, level, bonus);
        break;
        case 12:
          tmp = 1 + (RANDOM()%(10*bonus));
          if(bonus<0)
              op->stats.dam-=tmp;
          else
          {
            op->stats.dam+=tmp;
            if((RANDOM()%20) > 16)
            {
                op->stats.dam+=(RANDOM() % 9);
            }
          }
          break;
        case 13:
          op->stats.wc += bonus;
          if (bonus > 0 && (RANDOM() % 20) > 16)
          {
              op->stats.wc++;
          }
          break;
        case 14:
          op->stats.ac += bonus;
          if (bonus > 0 && (RANDOM() % 20) > 16)
          {
              op->stats.ac++;
          }
          break;
        default:
          if (!bonus)
              bonus = 1;
          op->stats.exp += bonus; /* Speed! */
          break;
    }
    if (op->value < 0) /* check possible overflow */
        op->value = 0;
    return 1;
}

/*
 * get_magic(diff) will return a random number between 0 and 4.
 * diff can be any value above 2.  The higher the diff-variable, the
 * higher is the chance of returning a low number.
 * It is only used in fix_generated_treasure() to set bonuses on
 * rings and amulets.
 * Another scheme is used to calculate the magic of weapons and armours.
 */
int get_magic(int diff)
{
    int i;
    if (diff < 3)
        diff = 3;
    for (i = 0; i < 4; i++)
        if (RANDOM() % diff)
            return i;
    return 4;
}

/* get a random spell from the spelllist.
 * used for item generation which uses spells.
 */
static int get_random_spell(int level, int flags)
{
    int i, tmp = RANDOM() % NROFREALSPELLS;

    /* we start somewhere random in the list and get the first fitting spell */
    /* spell matches when: is active, spell level is same or lower as difficuly/level
     * of mob or map and the flags matches.
     */
    for (i = tmp; i < NROFREALSPELLS; i++)
    {
        if (spells[i].is_active && level >= spells[i].level && spells[i].spell_use & flags)
            return i;
    }
    for (i = 0; i < tmp; i++)
    {
        if (spells[i].is_active && level >= spells[i].level && spells[i].spell_use & flags)
            return i;
    }

    /* if we are here, there is no fitting spell */
    return SP_NO_SPELL;
}

#define DICE2   (get_magic(2)==2?2:1)
#define DICESPELL (RANDOM()%3+RANDOM()%3+RANDOM()%3+RANDOM()%3+RANDOM()%3)

/*
 * fix_generated_item():  This is called after an item is generated, in
 * order to set it up right.  This produced magical bonuses, puts spells
 * into scrolls/books/wands, makes it unidentified, hides the value, etc.
 */
/* 4/28/96 added creator object from which op may now inherit properties based on
 * op->type. Right now, which stuff the creator passes on is object type
 * dependant. I know this is a spagetti manuever, but is there a cleaner
 * way to do this? b.t. */

// so this is where we have most the dynamics on item creation
int fix_generated_item(object_t **op_ptr, object_t *creator, int difficulty, int a_chance, int t_style, int max_magic,
                       int chance_magic, int flags)
{
    object_t *op  = *op_ptr; /* just to make things easy */
    int     temp, retval = 0, was_magic = op->magic;
    int     too_many_tries = 0, is_special = 0;
    int     fix_magic = 0;

    if (difficulty < 1)
        difficulty = 1;

    if(chance_magic==T_MAGIC_CHANCE_UNSET)
        chance_magic = 6;
    else if(chance_magic==100 && max_magic > 0)
        fix_magic = ABS(max_magic);

    if(max_magic == T_MAGIC_UNSET) /* default setting: adjust the magic boni to diff level */
    {
        if(difficulty > 79)
            max_magic = 3;
        else if(difficulty > 37)
            max_magic = 2;
        else
            max_magic = 1;
    }

    if (!creator || creator->type == op->type)
        creator = op; /*safety & to prevent polymorphed objects giving attributes */

    if (op->type != POTION && op->type != SCROLL)
    {
        if ((!op->magic && max_magic) || fix_magic || chance_magic==100)
            set_magic(difficulty, op, max_magic, fix_magic, chance_magic, flags);
        if (a_chance != 0)
        {
            if ((!was_magic && !(RANDOM() % CHANCE_FOR_ARTIFACT))
                 || op->type == HORN
                 || difficulty >= 999
                 || ((RANDOM() % 100) + 1) <= (a_chance==ART_CHANCE_UNSET?4:a_chance) )
                retval = generate_artifact(op, difficulty, t_style, a_chance);
        }
    }

    if (!op->title || op->type == RUNE) /* Only modify object if not special */
    {
        switch (op->type)
        {
            // looks like we get our scrolls now only from artifacts
            case SCROLL:
              // check first, if we get a scroll from artifacts
              while (op->stats.sp == SP_NO_SPELL)
              {
                  generate_artifact(op, difficulty, t_style, 100);
                  if (too_many_tries++ > 3)
                      break;
              }
              if (op->stats.sp == SP_NO_SPELL)
                  break;

              // if((op->stats.sp = get_random_spell(difficulty,SPELL_USE_SCROLL)) == SP_NO_SPELL) break;

              SET_FLAG(op, FLAG_IS_MAGICAL); // marks at magical
              op->stats.food = RANDOM() % spells[op->stats.sp].charges + 1; // charges
              // temp = (((difficulty * 100) - (difficulty * 20)) + (difficulty * (RANDOM() % 35))) / 100;
              temp = (0.8f + RANDOM()%35 / 100.0f) * difficulty;
              if (temp < 1)
                  temp = 1;
              else if (temp > 110)
                  temp = 110;
              op->level = temp;
              if (temp < spells[op->stats.sp].level)
                  temp = spells[op->stats.sp].level;
              /* op->value = (int) (85.0f * spells[op->stats.sp].value_mul);*/
              /*op->nrof=RANDOM()%spells[op->stats.sp].scrolls+1;*/
              break;

            case POTION:
              {
                  if (!op->subtype) /* balm */
                  {
                      if ((op->stats.sp = get_random_spell(difficulty, SPELL_USE_BALM)) == SP_NO_SPELL)
                          break;
                      SET_FLAG(op, FLAG_IS_MAGICAL);
                      op->value = (int) (150.0f * spells[op->stats.sp].value_mul);
                  }
                  else if (op->subtype > 128) /* dust */
                  {
                      if ((op->stats.sp = get_random_spell(difficulty, SPELL_USE_DUST)) == SP_NO_SPELL)
                          break;
                      SET_FLAG(op, FLAG_IS_MAGICAL);
                      op->value = (int) (125.0f * spells[op->stats.sp].value_mul);
                  }
                  else
                  {
                      while (!(is_special = op->last_eat) && op->stats.sp == SP_NO_SPELL)
                      {
                          generate_artifact(op, difficulty, t_style, 100);
                          if (too_many_tries++ > 3)
                              goto jump_break1;
                      }
                  }

                  //temp = (((difficulty * 100) - (difficulty * 20)) + (difficulty * (RANDOM() % 35))) / 100;
                  temp = (0.8f + RANDOM()%35 / 100.0f) * difficulty;
                  if (temp < 1)
                      temp = 1;
                  else if (temp > 110)
                      temp = 110;
                  if (!is_special && temp < spells[op->stats.sp].level)
                      temp = spells[op->stats.sp].level;

                  op->level = temp;

                  /* chance to make special potions damned or cursed.
                         * The chance is somewhat high to make the game more
                         * difficult. Applying this potions without identify
                         * is a great risk!
                         */
                  if (is_special && !(flags & GT_ONLY_GOOD))
                  {
                      if (RANDOM() % 2)
                          SET_FLAG(op, FLAG_CURSED);
                      else if (RANDOM() % 2)
                          SET_FLAG(op, FLAG_DAMNED);
                  }

                  jump_break1:
                  break;
              }

            case AMULET:
              if (op->arch == archetype_global._amulet_generic)
                // everywhere hardcode adjustments,
                // value should not be adjusted here. :-(
                  op->value *= 5; /* Since it's not just decoration */

            case RING:
              if (op->arch == NULL)
              {
                  remove_ob(op);
                  *op_ptr = op = NULL;
                  break;
              }

              if (op->arch != archetype_global._ring_generic && op->arch != archetype_global._amulet_generic) /* It's a special artefact!*/
                  break;

              /* We have no special ring - now we create one.
                     * we first get us a value, material & face
                     * changed prototype. Then we cast the powers over it.
                     */
              if (op->arch == archetype_global._ring_generic)
              {
                  if (!QUERY_FLAG(op, FLAG_REMOVED))
                      remove_ob(op); /* this is called before we inserted it in the map or elsewhere */
                  *op_ptr = op = arch_to_object(archetype_global._ring_normal);
                  generate_artifact(op, difficulty, t_style, 99);
              }

              if (!(flags & GT_ONLY_GOOD) && !(RANDOM() % 3))
                  SET_FLAG(op, FLAG_CURSED);
              set_ring_bonus(op, QUERY_FLAG(op, FLAG_CURSED) ? -DICE2 : DICE2, difficulty);

              // we are in generate ring, why we need amulet check here?
              if (!(RANDOM() % 4) && op->type != AMULET) /* Amulets have only one ability */
              {
                  int   d   = (RANDOM() % 2 || QUERY_FLAG(op, FLAG_CURSED)) ? -DICE2 : DICE2;
                  if (set_ring_bonus(op, d, difficulty))
                  if (!(RANDOM() % 4))
                  {
                      int   d   = (RANDOM() % 3 || QUERY_FLAG(op, FLAG_CURSED)) ? -DICE2 : DICE2;
                      set_ring_bonus(op, d, difficulty);
                  }
              }

              set_ring_bonus_value_calc(op);
              break;

            case BOOK:
              /* Is it an empty book?, if yes lets make a special
                    * msg for it, and tailor its properties based on the
                    * creator and/or map level we found it on.
                    */
              if (!op->arch->base_clone && !op->title && !op->msg && RANDOM() % 10)
              {
                  /* set the book level properly */
                  if (creator->level == 0)
                  {
                      if (op->map && op->map->difficulty)
                          op->level = RANDOM() % (op->map->difficulty) + RANDOM() % 10 + 1;
                      else
                          op->level = RANDOM() % 20 + 1;
                  }
                  else
                      op->level = RANDOM() % creator->level;

                  tailor_readable_ob(op, 0);
                  generate_artifact(op, 1,T_STYLE_UNSET, 100);
                  /* books w/ info are worth more! */
                  op->value += (int)((float)(((op->level > 10 ? op->level : (op->level + 1) / 2) * ((strlen(op->msg) / 45) + 1)))*1.11);
                  /* creator related stuff */
                  /* for library, chained books! */
                  /*if (QUERY_FLAG(creator, FLAG_NO_PICK))
                      SET_FLAG(op, FLAG_NO_PICK);*/
                  if (creator->slaying && !op->slaying) /* for check_inv floors */
                  {
                      SHSTR_FREE_AND_ADD_STRING(op->slaying, creator->slaying);
                  }
              }
              break;

            case SPELLBOOK:
              LOG(llevDebug, "DEBUG: fix_generated_system() called for disabled object SPELLBOOK (%s)\n", STRING_OBJ_NAME(op));
              break;

            case WAND:
              if ((op->stats.sp = get_random_spell(difficulty, SPELL_USE_WAND)) == SP_NO_SPELL)
                  break;

              SET_FLAG(op, FLAG_IS_MAGICAL); /* marks at magical */
              op->stats.food = (RANDOM() % spells[op->stats.sp].charges + 1) + 12; /* charges */

              //temp = (((difficulty * 100) - (difficulty * 20)) + (difficulty * (RANDOM() % 35))) / 100;
              temp = (0.8f + RANDOM()%35 / 100.0f) * difficulty;
              if (temp < 1)
                  temp = 1;
              else if (temp > 110)
                  temp = 110;
              if (temp < spells[op->stats.sp].level)
                  temp = spells[op->stats.sp].level;
              op->level = temp;
              op->value = (int) (16.3f * spells[op->stats.sp].value_mul);
              break;

            case HORN:
              if ((op->stats.sp = get_random_spell(difficulty, SPELL_USE_HORN)) == SP_NO_SPELL)
                  break;
              SET_FLAG(op, FLAG_IS_MAGICAL); /* marks at magical */
              if (op->stats.maxhp)
                  op->stats.maxhp += RANDOM() % op->stats.maxhp;
              op->stats.hp = op->stats.maxhp;

              //temp = (((difficulty * 100) - (difficulty * 20)) + (difficulty * (RANDOM() % 35))) / 100;
              temp = (0.8f + RANDOM()%35 / 100.0f) * difficulty;
              if (temp < 1)
                  temp = 1;
              else if (temp > 110)
                  temp = 110;
              op->level = temp;
              op->item_level = temp;
              op->item_skill = SKILLGROUP_MAGIC + 1;
              if (temp < spells[op->stats.sp].level)
                  temp = spells[op->stats.sp].level;
              op->value = (int) (1850.0f * spells[op->stats.sp].value_mul);
              break;

            case ROD:
              if ((op->stats.sp = get_random_spell(difficulty, SPELL_USE_ROD)) == SP_NO_SPELL)
                  break;
              SET_FLAG(op, FLAG_IS_MAGICAL); /* marks at magical */
              if (op->stats.maxhp)
                  op->stats.maxhp += RANDOM() % op->stats.maxhp;
              op->stats.hp = op->stats.maxhp;
              //temp = (((difficulty * 100) - (difficulty * 20)) + (difficulty * (RANDOM() % 35))) / 100;
              temp = (0.8f + RANDOM()%35 / 100.0f) * difficulty;
              if (temp < 1)
                  temp = 1;
              else if (temp > 110)
                  temp = 110;
              op->level = temp;
              op->item_level = temp;
              op->item_skill = SKILLGROUP_MAGIC + 1;
              if (temp < spells[op->stats.sp].level)
                  temp = spells[op->stats.sp].level;
              op->value = (int) (1850.0f * spells[op->stats.sp].value_mul);
              break;

            case RUNE:
              trap_adjust(op, difficulty); /* artifact AND normal treasure runes!! */
              break;
        } /* end switch */
    }
    else /* ->title != NULL */
    {
        switch (op->type)
        {
              /* lets check we have a slaying/assassination arrow */
            case ARROW:
              if (op->slaying == shstr_cons.none) /* compare hash ptrs */
              {
                  int           tmp = RANDOM() % global_race_counter;
                  racelink_t     *list;

                  /* get the right race */
                  for (list = first_race; list && tmp; list = list->next, tmp--)
                      ;
                  SHSTR_FREE_AND_ADD_STRING(op->slaying, list->name);
              }
              break;
        }
    }

    if (flags & GT_NO_VALUE && op->type != MONEY)
        op->value = 0;

    if (flags & GT_NO_DROP)
    {
        if (op->nrof < 2 && op->type != CONTAINER && op->type != MONEY && !QUERY_FLAG(op, FLAG_IS_THROWN))
            SET_FLAG(op, FLAG_NO_DROP);
        else if (op->type != MONEY)
            op->value = 0;
    }

    if (flags & GT_IDENTIFIED)
    {
        THING_IDENTIFY(op);
    }

    return retval;
}

/*
 * For debugging purposes.  Dumps all treasures recursively (see below).
 */
void dump_monster_treasure_rec(const char *name, treasure_t *t, int depth)
{
    treasurelist_t   *tl;
    int             i;

    if (depth > 100)
        return;
    while (t != NULL)
    {
        if (t->name != NULL)
        {
            for (i = 0; i < depth; i++)
                LOG(llevInfo, "  ");
            LOG(llevInfo, "{   (list: %s)\n", t->name);
            tl = find_treasurelist(t->name);
            dump_monster_treasure_rec(name, tl->items, depth + 2);
            for (i = 0; i < depth; i++)
                LOG(llevInfo, "  ");
            LOG(llevInfo, "}   (end of list: %s)\n", t->name);
        }
        else
        {
            for (i = 0; i < depth; i++)
                LOG(llevInfo, "  ");
            LOG(llevInfo, "%s\n", t->item->clone.name);
        }
        if (t->next_yes != NULL)
        {
            for (i = 0; i < depth; i++)
                LOG(llevInfo, "  ");
            LOG(llevInfo, " (if yes)\n");
            dump_monster_treasure_rec(name, t->next_yes, depth + 1);
        }
        if (t->next_no != NULL)
        {
            for (i = 0; i < depth; i++)
                LOG(llevInfo, "  ");
            LOG(llevInfo, " (if no)\n");
            dump_monster_treasure_rec(name, t->next_no, depth + 1);
        }
        t = t->next;
    }
}

void free_treasurestruct(treasure_t *t)
{
    if (t->next)
        free_treasurestruct(t->next);
    if (t->next_yes)
        free_treasurestruct(t->next_yes);
    if (t->next_no)
        free_treasurestruct(t->next_no);
    SHSTR_FREE(t->name);
    SHSTR_FREE(t->change_arch.name);
    SHSTR_FREE(t->change_arch.race);
    SHSTR_FREE(t->change_arch.slaying);
    SHSTR_FREE(t->change_arch.title);
    free(t);
}

void free_all_treasures()
{
    treasurelist_t   *tl, *next;


    for (tl = first_treasurelist; tl != NULL; tl = next)
    {
        next = tl->next;
        SHSTR_FREE(tl->listname);
        if (tl->items)
            free_treasurestruct(tl->items);
        free(tl);
    }
    first_treasurelist = NULL;

    free_artifactlist(first_artifactlist);
}

// i splitted this logic from set_material_real
int derive_material_real_from_material(int material)
{
    if (material & M_IRON)
        return M_START_IRON;
    else if (material & M_LEATHER)
        return M_START_LEATHER;
    else if (material & M_PAPER)
        return M_START_PAPER;
    else if (material & M_GLASS)
        return M_START_GLASS;
    else if (material & M_WOOD)
        return M_START_WOOD;
    else if (material & M_ORGANIC)
        return M_START_ORGANIC;
    else if (material & M_STONE)
        return M_START_STONE;
    else if (material & M_CLOTH)
        return M_START_CLOTH;
    else if (material & M_ADAMANT)
        return M_START_ADAMANT;
    else if (material & M_LIQUID)
        return M_START_LIQUID;
    else if (material & M_SOFT_METAL)
        return M_START_SOFT_METAL;
    else if (material & M_BONE)
        return M_START_BONE;
    else if (material & M_ICE)
        return M_START_ICE;
    else
        return M_NONE;
}

// i splitted this logic from set_material_real
// generates a random range with higher chances for lower qualities
int random_quality(int quality, int range)
{
    int mat_weight;

    if (range > 0)
    {
        // we loop from last to first
        for (int i = range; i > 0; i--)
        {
            // our rnd is always 0 to range-1
            int rnd = RANDOM() % range;
            if (rnd <= range / i) //higher the range or higher the loop(i), lower the chance
            {
                quality += range - i;
                break;
            }
        }
    }
    return quality;
}

 /// function set_material_real

 // we call this from 4 positions, all from treasure.c
 //	Line 1089: set_material_real(tmp, &t->change_arch); // from create_all_treasures
 //	Line 1093: set_material_real(tmp, change_arch); // from create_all_treasures
 //	Line 1250: set_material_real(tmp, &t->change_arch); // create_one_treasure
 //	Line 1254: set_material_real(tmp, change_arch); // create_one_treasure

 /// TODO we really need this double calls for one item ???

// call this function with the object to change in relation to treasurelist definitions
// change_arch->material_quality, change_arch->material, change_arch->material_range ..
// .. are set to -1 in function set_change_arch
// explicit definitions of this values to 0 in treasure list is also 0 here
/// so NO definitions in treasurelist comes here with -1
/// where 0 definitions in treasurelist comes here with 0

// op->material_real == -1 skips the logic
// change_arch->material == -1 tries to derive an op->material_real value
// change_arch->material != -1 overwrite the op->material_real value
// then if we have op->material_real>0 (0 is no material_real)
// ->adjust change_arch->material_range if we have >= NROFMATERIALS_REAL (new autoadjust logic)
// ->adjust op->material_real with random range (same chances for all ranges)
// if we have change_arch->material_quality != -1
// -> get a random quality, where higher qualities have lower chances to roll
// -> and if we have no material_real (op->material_real == 0)
// ---> simply overwrite op->item_quality with this random quality
// --> or
// ---> find the closest material to this random quality value and get the quality from there
// ----> but only do this for compatible materials, if not use the simply overwrite logic from above
// finally if we have also change_arch->quality !=-1 we overwrite with this value
// if not we overwrite with our material.quality value
// and last if we have change_arch->quality_range
// we adjust quality also by random range here ((same chances for all ranges)

static inline void set_material_real(object_t *op, struct _change_arch *change_arch)
{
    LOG(llevDebug,"op->material_real %d change_arch->material_quality %d change_arch->material %d change_arch->material_range %d\n",
         op->material_real, change_arch->material_quality, change_arch->material, change_arch->material_range);

    /// adjusting item_race in set_material_real, should be transfered somewhere else TODO
    if (change_arch->item_race != -1)
        op->item_race = (uint8) change_arch->item_race;

    op->item_condition = 100; /// TODO transfer this somewhere else

    /* this must be tested - perhaps we want that change_arch->material
     * also overrule the material_real -1 marker?
     */
    if (op->material_real == -1) // skip all objects with -1 as marker
    {
        /* WARNING: material_real == -1 skips also the quality modifier.
        * this is really for objects which don't fit in the material/quality
        * system (like system objects, forces, effects and stuff). */
        op->material_real = 0; // undefinied material
        return;
    }

    if (change_arch->material == -1) /// !!! this means in treasurelist material_real !!!
    {
      // normally this is a skip, but when we have no material_real (books, food) we try to derive this from material flag
      // if (!pointer) <- this is bad style to use on integer to check for 0
      // if (pointer == NULL)
      // if (pointer == 0) <- this would be normaly clearest to understand, when we have 100% integer value at pointer adress
      // so this means, when material_real==0 and material!=0 and material!= adamant
      if (!op->material_real && op->material && op->material != M_ADAMANT) // why extra check for adamant?
      {
        // derive function has one difference from old logic -> when not find a material_real it sets material_real to 0
        op->material_real = derive_material_real_from_material(op->material);
      }
    }
    else // change_arch->material != -1 /// !!! this means in treasurelist material_real !!!
    {
      op->material_real = change_arch->material; // allow to overwrite also 0 to material_real?
    }

    LOG(llevDebug,"*op->material_real %d change_arch->material_quality %d change_arch->material %d change_arch->material_range %d\n",
        op->material_real, change_arch->material_quality, change_arch->material, change_arch->material_range);

    if (op->material_real > 0)
    {
      // adjust range automated, when it is >= NROFMATERIALS_REAL (64)
      if (change_arch->material_range >= NROFMATERIALS_REAL)
      {
        // 0 is no material and we have blocks of NROFMATERIALS_REAL=64 behind this
        // we need to find our material block, we truncate material_real / NROFMATERIALS_REAL
        // and calc from there back
        int first=(int)(op->material_real / NROFMATERIALS_REAL) * NROFMATERIALS_REAL + 1;
        change_arch->material_range=first; // to be save to remove this 64
        for (int i = 1; i < NROFMATERIALS_REAL; i++)
        {
          if (material_real[first+i].quality==0) // if (strcmp(material_real[first+i].name,"")==1)
          {
            break; // stop searching
          }
          change_arch->material_range=i;
        }
      }

      // now adjust range to material_real if we have, placed it here, is a change now, to old logic because
      // - it's allow the 64 logic when we had change_arch->material !=-1
      // - and it's allow the range change also on books, etc, where we devided the material_real before
      if (change_arch->material_range > 0)
      {
        op->material_real += (RANDOM() % (change_arch->material_range + 1));
      }
    }

    LOG(llevDebug,"**op->material_real %d change_arch->material_quality %d change_arch->material %d change_arch->material_range %d\n",
      op->material_real, change_arch->material_quality, change_arch->material, change_arch->material_range);

    // if we have an explicit order to change the material_quality definition
    // we need also care about a change material_range order
    if (change_arch->material_quality != -1)
    {
        int rnd_quality = random_quality(change_arch->material_quality, change_arch->material_range);

        if (!op->material_real) // if (op->material_real == 0)
        {
            /// item_quality is quality in object, but is different in change_arch ///
            op->item_quality = rnd_quality; // no material_real, we can only adjust item_quality
            return;
        }

        // look in material_real table, search the nearest material to this quality
        if (op->material_real) // if (op->material_real !=0)
        {
            int first=(int)(op->material_real / NROFMATERIALS_REAL) * NROFMATERIALS_REAL + 1; // calc material block start

            // some material_real stuff works difference, like organics (which defines scales, chitin and stuff)
            // TODO we should add paper here too later?
            if (first == M_START_IRON || first == M_START_WOOD || first == M_START_LEATHER || first == M_START_CLOTH || first == M_START_STONE)
            {
                int max = first + NROFMATERIALS_REAL;  // NROFMATERIALS_REAL=64
                int tmp = material_real[first].quality;

                // loop ... and search for "closest" material in material_real, but not higher than our random range
                for (int i = first; i < max; ++i)
                {
                    int delta=rnd_quality - material_real[i].quality;

                    if (delta>=0)
                    {
                      if (rnd_quality - tmp > delta)
                      {
                        first = i; // memory this index
                        tmp = material_real[i].quality; // memory the quality to this index
                      }
                    }
                }
                op->material_real = first; // take this index
            }
            else // no compatible material
            {
                op->item_quality = rnd_quality;
                return;
            }
        }
    }

    LOG(llevDebug,"***op->material_real %d change_arch->material_quality %d change_arch->material %d change_arch->material_range %d\n",
      op->material_real, change_arch->material_quality, change_arch->material, change_arch->material_range);

    // adjust quality - use material default value or quality adjustment
    if (change_arch->quality != -1)
        op->item_quality = change_arch->quality;
    else
        op->item_quality = material_real[op->material_real].quality;

    if (change_arch->quality_range > 0)
    {
        op->item_quality += RANDOM() % (change_arch->quality_range + 1);
        if (op->item_quality > 100)
            op->item_quality = 100;
    }
}

/*
 * For debugging purposes.  Dumps all treasures for a given monster.
 * Created originally by Raphael Quinet for debugging the alchemy code.
 */
void dump_monster_treasure(const char *name)
{
    archetype_t  *at;
    int         found;

    found = 0;
    LOG(llevInfo, "\n");
    for (at = first_archetype; at != NULL; at = at->next)
    {
        if (!strcasecmp(at->name, name))
        {
            LOG(llevInfo, "treasures for %s (arch: %s)\n", at->clone.name, at->name);

            if (at->clone.randomitems != NULL)
            {
                struct objectlink_t   *ol;

                for (ol = at->clone.randomitems; ol; ol = ol->next)
                    dump_monster_treasure_rec(at->clone.name, ol->objlink.tl->items, 1);
            }
            else
                LOG(llevInfo, "(nothing)\n");
            LOG(llevInfo, "\n");
            found++;
        }
    }
    if (found == 0)
        LOG(llevInfo, "No objects have the name %s!\n\n", name);
}

/* these function fetch the "enviroment level" for
 * treasure generation for the given object.
 * It checks first the object itself has a level.
 * If not, it checks the object is in a map.
 * If so, it use the map level.
 * If not, it recursive checks then ->env for level.
 * If there is none, it checks the last env maps.
 * If there is none, it use level 1 as default.
 * That gives us always a valid level.
 * This function never returns a value <1 !
 */
int get_enviroment_level(object_t *op)
{
    object_t *env;

    if (!op)
    {
        LOG(llevBug, "get_enviroment_level() called for NULL object!\n");
        return 1;
    }


    /* return object level or map level... */
    if (op->level)
        return op->level;
    if (op->map)
        return op->map->difficulty ? op->map->difficulty : 1;

    /* ok, its not so easy... lets check for env */

    env = op->env;
    while (env)
    {
        if (env->level)
            return env->level;
        if (env->map)
            return env->map->difficulty ? env->map->difficulty : 1;
        env = env->env;
    }

    /* if we are here there is nothing which gives
     * us a valid level or map difficulty...
     * we give up and return a simple level 1.
     * note: this don't *must* be a bug or error.
     * There are possible setups where this value 1
     * its the right value.
     */

    return 1;
}
