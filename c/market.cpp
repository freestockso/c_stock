#include "market.h"


//confirm: is include HY data
void   uc_Market::Init(void)
{
	int ret;
	char file_source[64] = "..\\data\\tdx_gongsi.txt" ;
	char readbuf[1024];	
	ifstream if_file(file_source);		
	uc_GongSi *pgongsi;


	while(if_file >> readbuf)
	{
		if (!strcmp(readbuf,"[END]") )
			break;

		pgongsi = &gongsi[gongsi_size];
		ret = pgongsi->Init(readbuf);
		if (ret == 0)
		{
			if_file >> pgongsi->name;
			gongsi_size++;
		}	
		if_file.getline(readbuf, sizeof readbuf);   
	}

	pgongsi = &gongsi[gongsi_size];
	pgongsi->Init("399101");
	strcpy(pgongsi->name, "中小");
	gongsi_size++;

	pgongsi = &gongsi[gongsi_size];
	pgongsi->Init("399102");
	strcpy(pgongsi->name, "创业");
	gongsi_size++;

	pgongsi = &gongsi[gongsi_size];
	pgongsi->Init("399300");
	strcpy(pgongsi->name, "三百");
	gongsi_size++;
	
	

}

void   uc_Market::Cal_m1(void)
{
    int i;
    uc_GongSi *pgongsi;

    
    for (i=0; i<gongsi_size; i++)
    {
         pgongsi = &gongsi[i];
         if (pgongsi->type == GONGSITYPE_HANGYE)
            Cal_HY(pgongsi);
         if (pgongsi->type == GONGSITYPE_ZHISHU)
            Cal_HY(pgongsi);
    }

	printf("output_view info ..........................\n");
	for (i=0; i<gongsi_size; i++)
	{
		pgongsi = &gongsi[i];
		if (pgongsi->type != GONGSITYPE_NULL)
			pgongsi->Output_view();
    }
    
    return;
}

void   uc_Market::out_m1(void)
{
    out_m1_one();
    out_m1_hangye();
    out_m1_czg();
    out_m1_one_s();
    out_m1_hangye_s();
    out_m1_czg_s();    
}




int   uc_Market::is_m1_in_one(uc_GongSi *pgongsi)
{
    static char one[10][10] = {
        "000418", 
        "600298", 
        "002508", 
        "000596", 
        "000858", 
        "002557", 
        "002304", 
        "002661", 
        "600276", 
        "600519" 
        };
    int i;

    for (i=0; i<10; i++)
    {
        if (!strcmp(pgongsi->code, one[i]))
            return YES;
    }
    return NO;
}

void   uc_Market::out_m1_czg(void)
{
    uc_GongSi *pgongsi, *phy;
    char file_write[] = "..\\data\\m1\\czg.txt";
	char buf[512];

    ofstream of_file(file_write);
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
    {
        if (pgongsi->Is_czg()== NO)
            continue;
        if (pgongsi->fenshu < 85)
            continue;
        if (pgongsi->pdayk[pgongsi->dayk_size-1].syl250 < 0)
            continue;
        phy = GetGongsi_byCode(pgongsi->GetHYCode());
        if (phy == NULL)
            continue;
        if (phy->pdayk[phy->dayk_size-1].syl250 <= 0)
            continue;
        {
            sprintf(buf, "%8s%10s[%8s]%10d%4.0f%122s%8.2f\n", pgongsi->code, pgongsi->name, phy->name,
                pgongsi->pdayk[pgongsi->dayk_size-1].date,
                pgongsi->fenshu,
                pgongsi->GetStr_syl30_czg(),
                pgongsi->pdayk[pgongsi->dayk_size-1].syl30);
            of_file << buf; 
       } 
    }

    return;
}

void   uc_Market::out_m1_czg_s(void)
{
    uc_GongSi *pgongsi, *phy;
    char file_write[] = "..\\data\\m1\\czg_s.txt";
	char buf[512];

    ofstream of_file(file_write);
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
    {
        if (pgongsi->Is_czg()== NO)
            continue;
        if (pgongsi->fenshu < 85)
            continue;
        if (pgongsi->pdayk[pgongsi->dayk_size-1].syl250 < 0)
            continue;
        phy = GetGongsi_byCode(pgongsi->GetHYCode());
        if (phy == NULL)
            continue;
        if (phy->pdayk[phy->dayk_size-1].syl250 <= 0)
            continue;
        {
            sprintf(buf, "%8s%10s[%8s]%10d%4.0f%62s%8.2f\n", pgongsi->code, pgongsi->name, phy->name,
                pgongsi->pdayk[pgongsi->dayk_size-1].date,
                pgongsi->fenshu,
                pgongsi->GetStr_syl30_czg()+60,
                pgongsi->pdayk[pgongsi->dayk_size-1].syl30);
            of_file << buf; 
       } 
    }

    return;
}


void   uc_Market::out_m1_one(void)
{
    uc_GongSi *pgongsi;
    char file_write[] = "..\\data\\m1\\one.txt";
	char buf[512];

    ofstream of_file(file_write);
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
    {
        if (is_m1_in_one(pgongsi) == YES)
        {
            sprintf(buf, "%8s%10s%10d%4.0f%122s%10.2f\n", pgongsi->code, pgongsi->name, 
                pgongsi->pdayk[pgongsi->dayk_size-1].date,
                pgongsi->fenshu,
                pgongsi->GetStr_syl30_one(),
                pgongsi->pdayk[pgongsi->dayk_size-1].syl30);
            of_file << buf; 
       } 
    }

    return;
}

void   uc_Market::out_m1_one_s(void)
{
    uc_GongSi *pgongsi;
    char file_write[] = "..\\data\\m1\\one_s.txt";
	char buf[512];

    ofstream of_file(file_write);
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
    {
        if (is_m1_in_one(pgongsi) == YES)
        {
            sprintf(buf, "%8s%10s%10d%4.0f%52s%10.2f\n", pgongsi->code, pgongsi->name, 
                pgongsi->pdayk[pgongsi->dayk_size-1].date,
                pgongsi->fenshu,
                pgongsi->GetStr_syl30_one()+70,
                pgongsi->pdayk[pgongsi->dayk_size-1].syl30);
            of_file << buf; 
       } 
    }

    return;
}


void   uc_Market::out_m1_hangye(void)
{
    char file_write[] = "..\\data\\m1\\hangye.txt";
	char buf[512];
	uc_GongSi *pgongsi;
	int i, index;

	ofstream of_file(file_write);
	pgongsi = phead;
	i = 1;

	//output zhishu info
    sprintf(buf, "#####--Total info--------------------##############################-----------------------\n");of_file << buf; 
    sprintf(buf, "[p120d/p120dt syl250] [p30d/p30dt syl30]\n");
    of_file << buf; 
	for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%10d%20s\n", i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].date,
        	pgongsi->GetStr_tag_zs());
        of_file << buf; 
        i++;
	}
	i = 1;
	for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;	        

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "        %4d%8s%10s%10d%20s\n", i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].date,
        	pgongsi->GetStr_tag_hy());
        of_file << buf; 
		i++;
		if (i > 10)
		    break;
	}


	//output zhishu info
    sprintf(buf, "\n\n#####--zhishu info--------------------##############################-----------------------\n");of_file << buf; 
    sprintf(buf, "QD30...[p30>p120, '+']\n");
    of_file << buf; 
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%8.2f%%%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].QD_120_1 * 100,
            pgongsi->GetStr_p30p120());
        of_file << buf; 
        i++;
	}
    
    sprintf(buf, "\nP30d value...[p30d>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_hy());
        of_file << buf; 
		i++;
	}

    sprintf(buf, "\nP30d trend...[p30d today>p30d yestoday, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_trend_hy());
        of_file << buf; 
		i++;
	}

    sprintf(buf, "\nsyl30 value...[syl30>0, '+'][syl30>10, '-']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl30,
            pgongsi->GetStr_syl30_hy());
        of_file << buf; 
		i++;
	}


    sprintf(buf, "\nsyl250 value...[syl250>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl250,
            pgongsi->GetStr_syl250_hy());
        of_file << buf; 
		i++;
	}

	
    //output hangye info
    sprintf(buf, "\n\n#####--hangye info-------------#############################------------------------------\n");of_file << buf; 
    sprintf(buf, "QD30...[p30>p120, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%8.2f%%%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].QD_120_1 * 100,
            pgongsi->GetStr_p30p120());
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nP30d value...[p30d>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_hy());
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nP30d trend...[p30d today>p30d yestoday, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_trend_hy());
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nsyl30 value...[syl30>0, '+'][syl30>10, '-']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl30,
            pgongsi->GetStr_syl30_hy());
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}


    sprintf(buf, "\nsyl250 value...[syl250>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%122s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl250,
            pgongsi->GetStr_syl250_hy());
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}


    //the top 5 gongsi for every hangye
    of_file << "\n\n##### Top5 for hangye#################\n";
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    uc_GongSi *pgs;
	    int num = 0;
	    
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;
        sprintf(buf, "%10s%10s...................................\n",  pgongsi->code, pgongsi->name);
        of_file << buf;

        for(pgs = phead; pgs != NULL; pgs=pgs->pnext_by_fenshu)   
        {
            if (pgs->type != GONGSITYPE_GONGSI)
                continue;
            if (pgs->Is_InHY(pgongsi->code) == NO)
                continue;
            if (pgs->name[0] == 'S')
                continue;
            if (pgs->name[0] == '*')
                continue;
            {
                sprintf(buf, "          %10s%10s%10d%4.0f%82s%8.2f\n",  pgs->code, pgs->name,
                	pgs->pdayk[pgs->dayk_size-1].date,
                	pgs->fenshu, pgs->GetStr_syl30_one()+ 40, 
                	pgs->pdayk[pgs->dayk_size-1].syl30);
                of_file << buf;
                num++;
                if (num >=5)
                    break;
            }
        }
        
		i++;
		if (i > 10)
		    break;
	}


}


void   uc_Market::out_m1_hangye_s(void)
{
    char file_write[] = "..\\data\\m1\\hangye_s.txt";
	char buf[512];
	uc_GongSi *pgongsi;
	int i, index;

	ofstream of_file(file_write);
	pgongsi = phead;
	i = 1;

	//output zhishu info
    sprintf(buf, "#####--Total info--------------------##############################-----------------------\n");of_file << buf; 
    sprintf(buf, "[p120d/p120dt syl250] [p30d/p30dt syl30]\n");
    of_file << buf; 
	for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%10d%20s\n", i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].date,
        	pgongsi->GetStr_tag_zs());
        of_file << buf; 
        i++;
	}
	i = 1;
	for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;	        

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "        %4d%8s%10s%10d%20s\n", i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].date,
        	pgongsi->GetStr_tag_hy());
        of_file << buf; 
		i++;
		if (i > 10)
		    break;
	}


	//output zhishu info
    sprintf(buf, "\n\n#####--zhishu info--------------------##############################-----------------------\n");of_file << buf; 
    sprintf(buf, "QD30...[p30>p120, '+']\n");
    of_file << buf; 
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%8.2f%%%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].QD_120_1 * 100,
            pgongsi->GetStr_p30p120()+60);
        of_file << buf; 
        i++;
	}
    
    sprintf(buf, "\nP30d value...[p30d>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_hy()+60);
        of_file << buf; 
		i++;
	}

    sprintf(buf, "\nP30d trend...[p30d today>p30d yestoday, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_trend_hy()+60);
        of_file << buf; 
		i++;
	}

    sprintf(buf, "\nsyl30 value...[syl30>0, '+'][syl30>10, '-']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl30,
            pgongsi->GetStr_syl30_hy()+60);
        of_file << buf; 
		i++;
	}


    sprintf(buf, "\nsyl250 value...[syl250>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_ZHISHU)
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl250,
            pgongsi->GetStr_syl250_hy()+60);
        of_file << buf; 
		i++;
	}

	
    //output hangye info
    sprintf(buf, "\n\n#####--hangye info-------------#############################------------------------------\n");of_file << buf; 
    sprintf(buf, "QD30...[p30>p120, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%8.2f%%%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].QD_120_1 * 100,
            pgongsi->GetStr_p30p120()+60);
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nP30d value...[p30d>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_hy()+60);
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nP30d trend...[p30d today>p30d yestoday, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.0f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].p30d,
            pgongsi->GetStr_p30d_trend_hy()+60);
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}

    sprintf(buf, "\nsyl30 value...[syl30>0, '+'][syl30>10, '-']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl30,
            pgongsi->GetStr_syl30_hy()+60);
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}


    sprintf(buf, "\nsyl250 value...[syl250>0, '+']\n");
    of_file << buf; 
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;

	    index = pgongsi->dayk_size-1;
        sprintf(buf, "%4d%8s%10s%9.2f%62s\n", 
            i, pgongsi->code, pgongsi->name, pgongsi->pdayk[index].syl250,
            pgongsi->GetStr_syl250_hy()+60);
        of_file << buf; 
        
		i++;
		if (i > 10)
		    break;
	}


    //the top 5 gongsi for every hangye
    of_file << "\n\n##### Top5 for hangye#################\n";
    i = 1;
    for(pgongsi = phead; pgongsi != NULL; pgongsi=pgongsi->pnext_by_fenshu)
	{
	    uc_GongSi *pgs;
	    int num = 0;
	    
	    if (pgongsi->type != GONGSITYPE_HANGYE)
	        continue;
	    if (pgongsi->num_gongsi_hy <= 5)
	        continue;
	    if (!strcmp(pgongsi->name, "NULL"))
	        continue;
        sprintf(buf, "%10s%10s...................................\n",  pgongsi->code, pgongsi->name);
        of_file << buf;

        for(pgs = phead; pgs != NULL; pgs=pgs->pnext_by_fenshu)   
        {
            if (pgs->type != GONGSITYPE_GONGSI)
                continue;
            if (pgs->Is_InHY(pgongsi->code) == NO)
                continue;
            if (pgs->name[0] == 'S')
                continue;
            if (pgs->name[0] == '*')
                continue;
            {
                sprintf(buf, "          %10s%10s%10d%4.0f%42s%8.2f\n",  pgs->code, pgs->name,
                	pgs->pdayk[pgs->dayk_size-1].date,
                	pgs->fenshu, pgs->GetStr_syl30_one()+ 80, 
                	pgs->pdayk[pgs->dayk_size-1].syl30);
                of_file << buf;
                num++;
                if (num >=5)
                    break;
            }
        }
        
		i++;
		if (i > 10)
		    break;
	}


}

void uc_Market::Cal_Paiming(void)
{
	int i,j,index, valid_gongsi_total;
	float qiangdu_max;
	uc_GongSi *pgongsi_max;
	uc_GongSi *pgongsi, *p_pre;

	phead = NULL;
	p_pre = NULL;
	for (i=0; i<gongsi_size; i++)
		gongsi[i].flag = NO;
	valid_gongsi_total = 0;


	//算出排名顺序
	for (i=0; i<gongsi_size; i++)
	{
		pgongsi_max = NULL;
		qiangdu_max = (float)-99;
		
		for (j=0; j<gongsi_size; j++)
		{
			if (gongsi[j].flag == YES)
				continue;
			index = gongsi[j].dayk_size - 1;
			if (gongsi[j].pdayk[index].QD_120_1 > qiangdu_max)
			{
				qiangdu_max = gongsi[j].pdayk[index].QD_120_1;
				pgongsi_max = &gongsi[j];
			}
		}
		
		if (pgongsi_max == NULL)		//没有有效公司了
			break;
		
		pgongsi_max->fenshu= (float)(i+1);
		pgongsi_max->flag = YES;
		
		valid_gongsi_total++;

		if (phead == NULL)
		{
			phead = pgongsi_max;
			p_pre = pgongsi_max;
			p_pre->pnext_by_fenshu = NULL; 
		}
		else
		{
			p_pre->pnext_by_fenshu = pgongsi_max;
			p_pre = pgongsi_max;
			p_pre->pnext_by_fenshu = NULL; 						
		}
	}

	//算出强度排名和平均120
	for (i=0; i<gongsi_size; i++)
	{
		pgongsi = &gongsi[i];
		if (pgongsi->flag == NO)		
			continue;
		pgongsi->fenshu = 100 - (pgongsi->fenshu * 100) / (float)valid_gongsi_total;
	}

#if 1
	//输出到文件，测试这个函数是否正确
	{
	    char file_write[] = "..\\data\\gongsi_pm.txt";
    	char buf[512];	

		ofstream of_file(file_write);
		pgongsi = phead;
		i = 1;

		while(pgongsi != NULL)
		{
			sprintf(buf, "%10d%10s%10s\n", i, pgongsi->code, pgongsi->name);
			of_file << buf; 
			i++;

			pgongsi = pgongsi->pnext_by_fenshu;
		}
	}
#endif 

}

uc_GongSi *uc_Market::GetGongsi_byCode(char* code)
{

    uc_GongSi *pgongsi;
    int i;

    for (i=0; i<gongsi_size; i++)
    {
        pgongsi = &gongsi[i];
        if (!strcmp(pgongsi->code, code))
            return pgongsi;
    }

    return NULL;
}


void uc_Market::Cal_HY(uc_GongSi *phy)
{
    int i_gc,i_date, date, index, num_gongsi, j;
    uc_GongSi *pgongsi;
    float total;

    printf("...Cal_HY %s... %s...\n", phy->code, phy->name);

    //get hy gongsi and set flag
    num_gongsi = 0;
    for (i_gc = 0; i_gc < gongsi_size; i_gc++)
    {
        pgongsi = &gongsi[i_gc];
        pgongsi->flag = NO;
        if (pgongsi->type != GONGSITYPE_GONGSI)
            continue;
        if  (pgongsi->Is_InHY(phy->code) == NO)
            continue;
        pgongsi->flag = YES;    
        num_gongsi++;
    }    
    phy->num_gongsi_hy = num_gongsi;
    if (num_gongsi == 0)
        return;
    
    //find every gongsi and get the right day data
    for (i_date=0; i_date<phy->dayk_size; i_date++)
    {
        date = phy->pdayk[i_date].date;
        phy->pdayk[i_date].p120d = (float)0;
        phy->pdayk[i_date].p30d = (float)0;
        phy->pdayk[i_date].yl250 = (float)0;
        phy->pdayk[i_date].yl30 = (float)0;
        phy->pdayk[i_date].cj250 = (float)0;
        phy->pdayk[i_date].cj30 = (float)0;
        num_gongsi = 0;
        
        for (i_gc = 0; i_gc < gongsi_size; i_gc++)
        {
            pgongsi = &gongsi[i_gc];
            if ( pgongsi->flag == NO)
                continue;

            index = pgongsi->Get_IndexByDate(date);
            if (index <= 0)
                continue;
            num_gongsi++;    
            phy->pdayk[i_date].p120d += pgongsi->pdayk[index].p120d;
            phy->pdayk[i_date].p30d += pgongsi->pdayk[index].p30d;
            phy->pdayk[i_date].yl250 += pgongsi->pdayk[index].yl250;
            phy->pdayk[i_date].yl30 += pgongsi->pdayk[index].yl30;            
            phy->pdayk[i_date].cj250 += pgongsi->pdayk[index].cj250;
            phy->pdayk[i_date].cj30 += pgongsi->pdayk[index].cj30;            
        }

        phy->pdayk[i_date].p120d = My_div(phy->pdayk[i_date].p120d, (float)num_gongsi);
        phy->pdayk[i_date].p30d = My_div(phy->pdayk[i_date].p30d, (float)num_gongsi);
        phy->pdayk[i_date].syl30 = My_div(phy->pdayk[i_date].yl30, phy->pdayk[i_date].cj30) * 100; 
        phy->pdayk[i_date].syl250 = My_div(phy->pdayk[i_date].yl250, phy->pdayk[i_date].cj250)  * 100; 
    }

    //pinghua
    for (i_date=phy->dayk_size-1; i_date>5; i_date--)
    {
        total = 0;
        for (j=0; j<5; j++)
            total = total + phy->pdayk[i_date-j].syl30;
        phy->pdayk[i_date].syl30 = total / 5;
    }
    for (i_date=phy->dayk_size-1; i_date>5; i_date--)
    {
        total = 0;
        for (j=0; j<5; j++)
            total = total + phy->pdayk[i_date-j].p30d;
        phy->pdayk[i_date].p30d = total / 5;
    }
    for (i_date=phy->dayk_size-1; i_date>30; i_date--)
    {
        total = 0;
        for (j=0; j<30; j++)
            total = total + phy->pdayk[i_date-j].p120d;
        phy->pdayk[i_date].p120d = total / 30;
    }
    for (i_date=phy->dayk_size-1; i_date>30; i_date--)
    {
        total = 0;
        for (j=0; j<30; j++)
            total = total + phy->pdayk[i_date-j].syl250;
        phy->pdayk[i_date].syl250= total / 30;
    }
}

