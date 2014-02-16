options mprint;

%let glist = '006066' '012141' '014489';
* %let glist = '006066' '012141' '014489' '001004' '001013' '001021' '001034' '001045' '001050' '001056' '001062' '001072' '001076';

%let begdate = "01JAN2005"d;
%let enddate = "31DEC2006"d;

/*
Earnings Yield = ((OIADPQ_t + OIADPQ_t-1 +OIADPQ_t-2 +OIADPQ_t-3)/((PRCCD*CSHOQ*AJEXQ) + PSTQ +DLTTQ + DLCQ –CHEQ)
<96> is a "-"
*/

* Earnings Yield ;
proc sql;
create table EY as
select A.gvkey, A.datadate, A.fyearq, A.OIADPQ, A.CSHOQ, A.AJEXQ, A.PSTKQ, A.DLTTQ, A.DLCQ, A.CHEQ, B.prccm
from comp.fundq A, comp.secm B
where (&begdate < A.datadate <= &enddate)
	and (B.gvkey in (&glist)) and A.gvkey = B.gvkey and A.datadate=B.datadate; 

proc print data=&syslast(obs=100); 
run;



/*
proc sql;
create table two as select A.*,
	case when pstkq lt 0 then 
		(OIADPQ/((PRCCM*CSHOQ*AJEXQ)+PSTKQ–CHEQ))
	else
		(OIADPQ/((PRCCM*CSHOQ*AJEXQ)+PSTKQ–CHEQ))
	end as Yield
from
	one A
order by gvkey;
*/

/*
proc sql;
create table two as select A.*, ( OIADPQ / ( ( PRCCM * CSHOQ * AJEXQ ) – CHEQ ) ) as Yield
from 
	one A
order by gvkey;
*/

data one; set EY;
	by gvkey ;
	retain countgvkey rollingyield;
	laggvkey = lag(gvkey);
	if first.gvkey then do;
		countgvkey = 1;
		rollingyield = 0;
	end;
	else do;
	if countgvkey = 4 then do;
		countgvkey = 1;
		if gvkey = laggvkey then do;
			rollingyield = 1; end;
	end;
	else do;
		countgvkey = countgvkey + 1; end;
	end;

	tmp1 = PRCCM * CSHOQ * AJEXQ ;
	if DLTTQ ne . then tmp2 = PSTKQ + DLTTQ ;
		else tmp2 = PSTKQ ;
	if DLCQ ne . then tmp3 = tmp2 + DLCQ ;
		else tmp3 = tmp2 ;
	tmp4 = tmp1 - tmp3 ;
	agg1 = OIADPQ ;
	agg2 = lag(OIADPQ) ;
	agg3 = lag2(OIADPQ) ;
	agg4 = lag3(OIADPQ) ;
	if countgvkey = 4 then agg5 = agg1 + agg2 + agg3 + agg4 ;
	if rollingyield = 1 then agg5 = agg1 + agg2 + agg3 + agg4 ;
*	if countgvkey = 4 then agg4 = OIADPQ + lag(OIADPQ) + lag2(OIADPQ) + lag3(OIADPQ) ;
	if countgvkey = 4 then Earnings_Yield = agg5 / tmp4 ;
	if rollingyield = 1 then Earnings_Yield = agg5 / tmp4 ;
run;


* case when A.CHEQ lt 0 then . else A.CHEQ / B.prccm end as BtoM from one A;

quit;

proc print data=&syslast(obs=100); 
run;


/*
Return on Tangible Capital Employed
ROTCE = ((OIADPQ_t + OIADPQ_t-1 +OIADPQ_t-2 +OIADPQ_t-3)/(PPENTQ + ACTQ –CHEQ – LCTQ +DLCQ)
*/


* second ;
proc sql;
create table ROTCE as
select A.gvkey, A.datadate, A.fyearq, A.OIADPQ, A.PPENTQ, A.ACTQ, A.CHEQ, A.LCTQ, A.DLCQ
from comp.fundq A
where (&begdate < A.datadate <= &enddate)
	and (A.gvkey in (&glist));

proc print data=&syslast(obs=100); 
run;

data two; set ROTCE;
	by gvkey ;
	retain countgvkey rollingyield;
	laggvkey = lag(gvkey);
	if first.gvkey then do;
		countgvkey = 1;
		rollingyield = 0;
	end;
	else do;
	if countgvkey = 4 then do;
		countgvkey = 1;
		if gvkey = laggvkey then do;
			rollingyield = 1; end;
	end;
	else do;
		countgvkey = countgvkey + 1; end;
	end;

	tmp1 = PPENTQ + ACTQ - CHEQ - LCTQ;
	if DLCQ ne . then tmp2 = tmp1 + DLCQ ;
		else tmp2 = tmp1 ;
/*
	if DLTTQ ne . then tmp2 = PSTKQ + DLTTQ ;
		else tmp2 = PSTKQ ;
	tmp4 = tmp1 - tmp3 ;
*/
	agg1 = OIADPQ ;
	agg2 = lag(OIADPQ) ;
	agg3 = lag2(OIADPQ) ;
	agg4 = lag3(OIADPQ) ;
	if countgvkey = 4 then agg5 = agg1 + agg2 + agg3 + agg4 ;
	if rollingyield = 1 then agg5 = agg1 + agg2 + agg3 + agg4 ;
*	if countgvkey = 4 then agg4 = OIADPQ + lag(OIADPQ) + lag2(OIADPQ) + lag3(OIADPQ) ;
	if countgvkey = 4 then ReturnOTCE = agg5 / tmp2 ;
	if rollingyield = 1 then ReturnOTCE = agg5 / tmp2 ;
run;


* case when A.CHEQ lt 0 then . else A.CHEQ / B.prccm end as BtoM from one A;

quit;

proc print data=&syslast(obs=100); 
run;

















/* DEBUGGING

data two; set one;
    by gvkey ;
    retain countgvkey rollingyield;
    laggvkey = lag(gvkey);
		if gvkey ne laggvkey then do; 
			whatever = 1; end;
		else do;
			rollyield = 1; end;
		if gvkey eq laggvkey then do; 
			whatever2 = 1; end;
		else do;
			rollyield2 = 1; end;
		if lag(whatever2) eq '.' then do;
			rollingyield3 = 1; end;
		else do;
			rollingyield4 = 1; end;
		if whatever2 eq 1 then do;
			rollingyield5 = 1; end;
		else do;
			rollingyield6 = 1; end;
		if whatever2 ne . then do;
			rollingyield7 = 1; end;
		else do;
			rollingyield8 = 1; end;
		if missing(whatever2) then rollingyield9 = 1 ;
    if first.gvkey then do;
        countgvkey = 0;
    end;
    else do;
	*if rollingyield = 1 then do;

	*end;
	if countgvkey = 4 then do;
		countgvkey = 9;
		if gvkey = laggvkey then do;
			rollingyield = 1; end;
	end;
	else do;
		countgvkey = countgvkey + 1; end;
    end;
*/
