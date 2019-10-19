#include "usmart.h"

U8 USART_RX_BUF[PARM_LEN+1];   
U16 USART_RX_STA=0;       

//////////////////////////////////////////////////////////////////////////////////	 
//升级说明
//V1.4
//增加了对参数为string类型的函数的支持.适用范围大大提高.
//优化了内存占用,静态内存占用为79个字节@10个参数.动态适应数字及字符串长度
//V2.0 
//1,修改了list指令,打印函数的完整表达式.
//2,增加了id指令,打印每个函数的入口地址.
//3,修改了参数匹配,支持函数参数的调用(输入入口地址).
//4,增加了函数名长度宏定义.	
//V2.1 20110707		 
//1,增加dec,hex两个指令,用于设置参数显示进制,及执行进制转换.
//注:当dec,hex不带参数的时候,即设定显示参数进制.当后跟参数的时候,即执行进制转换.
//如:"dec 0XFF" 则会将0XFF转为255,由串口返回.
//如:"hex 100" 	则会将100转为0X64,由串口返回
//2,新增usmart_get_cmdname函数,用于获取指令名字.
//V2.2 20110726	
//1,修正了void类型参数的参数统计错误.
//2,修改数据显示格式默认为16进制.
//V2.3 20110815
//1,去掉了函数名后必须跟"("的限制.
//2,修正了字符串参数中不能有"("的bug.
//3,修改了函数默认显示参数格式的修改方式. 
//V2.4 20110905
//1,修改了usmart_get_cmdname函数,增加最大参数长度限制.避免了输入错误参数时的死机现象.
//2,增加USMART_ENTIM2_SCAN宏定义,用于配置是否使用TIM2定时执行scan函数.
//V2.5 20110930
//1,修改usmart_init函数为void usmart_init(U8 sysclk),可以根据系统频率自动设定扫描时间.(固定100ms)
//2,去掉了usmart_init函数中的uart_init函数,串口初始化必须在外部初始化,方便用户自行管理.
//V2.6 20111009
//1,增加了read_addr和write_addr两个函数.可以利用这两个函数读写内部任意地址(必须是有效地址).更加方便调试.
//2,read_addr和write_addr两个函数可以通过设置USMART_USE_WRFUNS为来使能和关闭.
//3,修改了usmart_strcmp,使其规范化.			  
//V2.7 20111024
//1,修正了返回值16进制显示时不换行的bug.
//2,增加了函数是否有返回值的判断,如果没有返回值,则不会显示.有返回值时才显示其返回值.
//V2.8 20111116
//1,修正了list等不带参数的指令发送后可能导致死机的bug.
//V2.9 20120917
//1,修改了形如：void*xxx(void)类型函数不能识别的bug。
//V3.0 20130425
//1,新增了字符串参数对转义符的支持。
//V3.1 20131120
//1,增加runtime系统指令,可以用于统计函数执行时间.
//用法:
//发送:runtime 1 ,则开启函数执行时间统计功能
//发送:runtime 0 ,则关闭函数执行时间统计功能
/////////////////////////////////////////////////////////////////////////////////////
//系统命令
U8 *sys_cmd_tab[]=
{
	"?",
	"help",
	"list",
	"id",
	"hex",
	"dec",	   
};	    
//处理系统指令
//0,成功处理;其他,错误代码;
U8 usmart_sys_cmd_exe(U8 *str)
{
	U8 i;
	U8 sfname[MAX_FNAME_LEN];//存放本地函数名
	U8 pnum;
	U8 rval;
	U32 res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);//得到指令及指令长度
	if(res)return USMART_FUNCERR;//错误的指令 
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)//支持的系统指令
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:
		case 1://帮助指令
#if USMART_USE_HELP
			printf("------------------------USMART V3.1------------------------ \r\n");
			printf("USMART有7个系统命令:\r\n");
			printf("?:      获取帮助信息\r\n");
			printf("help:   获取帮助信息\r\n");
			printf("list:   可用的函数列表\r\n");
			printf("id:     可用函数的ID列表\r\n");
			printf("hex:    参数16进制显示,后跟空格+数字即执行进制转换\r\n");
			printf("dec:    参数10进制显示,后跟空格+数字即执行进制转换\r\n");
			printf("请按照程序编写格式输入函数名及参数并以回车键结束.\r\n");    
#else
			printf("指令失效\r\n");
#endif
			break;
		case 2://查询指令
			printf("-------------------------函数清单--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)printf("%s\r\n",usmart_dev.funs[i].name);
			printf("\r\n");
			break;	 
		case 3://查询ID
			printf("-------------------------函数 ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((U8*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//得到本地函数名 
				printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func); //显示ID
			}
			break;
		case 4://hex指令
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						  	//进制转换功能
				{
					printf("HEX:0x%x\r\n",res);	   	//转为16进制
				}else if(i!=4)return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					printf("16进制参数显示!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}

			}else return USMART_PARMERR;			//参数错误.
			break;
		case 5://dec指令
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						   	//进制转换功能
				{
					printf("DEC:%d\r\n",res);	   	//转为10进制
				}else if(i!=4)return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					printf("10进制参数显示!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}else return USMART_PARMERR;			//参数错误. 
			break;	 	    
		default://非法指令
			return USMART_FUNCERR;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//初始化串口控制器
void usmart_init(void)
{
#if USMART_ENTIMX_SCAN==1

#endif
	usmart_dev.sptype=1;	//十六进制显示参数
}		
//从str中获取函数名,id,及参数信息
//*str:字符串指针.
//返回值:0,识别成功;其他,错误代码.
U8 usmart_cmd_rec(U8*str) 
{
	U8 sta,i,rval;//状态	 
	U8 rpnum,spnum;
	U8 rfname[MAX_FNAME_LEN];//暂存空间,用于存放接收到的函数名  
	U8 sfname[MAX_FNAME_LEN];//存放本地函数名
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//得到接收到的数据的函数名及参数个数	  
	if(sta)return sta;//错误
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((U8*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//得到本地函数名及参数个数
		if(sta)return sta;//本地解析有误	 
		
		if(usmart_strcmp(sfname,rfname)==0)//相等
		{
			if(spnum>rpnum)return USMART_PARMERR;//参数错误(输入参数比源函数参数少)
			usmart_dev.id=i;//记录函数ID.
			break;//跳出.
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	//未找到匹配的函数
 	sta=usmart_get_fparam(str,&i);					//得到函数参数个数	
	if(sta)return sta;								//返回错误
	usmart_dev.pnum=i;								//参数个数记录
    return USMART_OK;
}
//usamrt执行函数
//该函数用于最终执行从串口收到的有效函数.
//最多支持10个参数的函数,更多的参数支持也很容易实现.不过用的很少.一般5个左右的参数的函数已经很少见了.
//该函数会在串口打印执行情况.以:"函数名(参数1，参数2...参数N)=返回值".的形式打印.
//当所执行的函数没有返回值的时候,所打印的返回值是一个无意义的数据.
void usmart_exe(void)
{
	U8 id,i;
	U32 res;		   
	U32 temp[MAX_PARM];//参数转换,使之支持了字符串 
	U8 sfname[MAX_FNAME_LEN];//存放本地函数名
	U8 pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;//不执行.
	usmart_get_fname((U8*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//得到本地函数名,及参数个数 
	printf("%s(",sfname);//输出正要执行的函数名
	for(i=0;i<pnum;i++)//输出参数
	{
		if(usmart_dev.parmtype&(1<<i))//参数是字符串
		{
			printf("%c",'"');			 
			printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			printf("%c",'"');
			temp[i]=(U32)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //参数是数字
		{
			temp[i]=*(U32*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC) printf("%d",temp[i]);//10进制参数显示
			else printf("0x%x",temp[i]);//16进制参数显示 	   
		}
		if(i!=pnum-1) printf(",");
	}
	printf(")\r\n");
	//usmart_reset_runtime();	//计时器清零,开始计时
	switch(usmart_dev.pnum)
	{
		case 0://无参数(void类型)											  
			res=(*(U32(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://有1个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://有2个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://有3个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://有4个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://有5个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://有6个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://有7个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://有8个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://有9个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://有10个参数
			res=(*(U32(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
	//usmart_get_runtime();//获取函数执行时间
	if(rval==1)//需要返回值.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC) printf("=%d;\r\n",res);//输出执行结果(10进制参数显示)
		else printf("=0x%x\r\n",res);//输出执行结果(16进制参数显示)	   
	}else printf("\r\n");		//不需要返回值,直接输出结束
	if(usmart_dev.runtimeflag)	//需要显示函数执行时间
	{ 
		printf("Function Run Time:%dms\r\n",usmart_dev.runtime);//打印函数执行时间 
	}	
}
//usmart扫描函数
//通过调用该函数,实现usmart的各个控制.该函数需要每隔一定时间被调用一次
//以及时执行从串口发过来的各个函数.
//本函数可以在中断里面调用,从而实现自动管理.
void usmart_scan(void)
{
	U8 sta, len;  

	len = (USART_RX_STA&(~0x8000))-1;
	if (len == 0) {
		printf("\r\n");
		return;
	} 
	
	USART_RX_BUF[len]='\0';	//在末尾加入结束符. 
	sta=usmart_dev.cmd_rec(USART_RX_BUF);//得到函数各个信息
	if(sta==0)usmart_dev.exe();	//执行函数 
	else 
	{  
		len=usmart_sys_cmd_exe(USART_RX_BUF);
		if(len!=USMART_FUNCERR)sta=len;
		if(sta)
		{
			switch(sta)
			{
				case USMART_FUNCERR:
					printf("函数错误!\r\n");   			
					break;	
				case USMART_PARMERR:
					printf("参数错误!\r\n");   			
					break;				
				case USMART_PARMOVER:
					printf("参数太多!\r\n");   			
					break;		
				case USMART_NOFUNCFIND:
					printf("未找到匹配的函数!\r\n");   			
					break;		
			}
		}
	}
}

/* ****************************
*		函数名: void usmart_test(void)
*		作用:		usmart测试函数
*		输入: 无
*		输出: 无
* ************************* */
void usmart_test(void) {
	printf("this is test fun\r\n");
}

/*****************USER**************************/
PROCESS(usmart_process, "usmart_scan");
/* ****************************
*		函数名: PROCESS_THREAD(usmart_process, ev, data)
*		作用:		usmart检测线程
*		输入: data
*		输出: 无
* ************************* */
PROCESS_THREAD(usmart_process, ev, data)
{
		static struct etimer et;
    PROCESS_BEGIN();
    while(1)
    {	
				if ((USART_RX_STA&0x8000) == 0x8000) {
					usmart_scan();
					USART_RX_STA = 0;
				}
        
				PT_E_DELAY(&et, CLOCK_SECOND/10);
    }

    PROCESS_END();
}

 
