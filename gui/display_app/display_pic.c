
#include "display_pic.h"
#include "ff.h"
#include "trace.h"
#include "hk_peripheral.h"
#include "piclib.h"
#include "little_malloc.h"

FILINFO picfile_info;            

uint16_t pic_get_tnum(uint8_t *path)
{	  
	uint8_t res;
	uint16_t rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息	
	uint8_t *fn;	 	
    
    res = f_opendir(&tdir, (const TCHAR*)path); 	             //打开目录
  	tfileinfo.lfsize = _MAX_LFN*2 + 1;				             //长文件名最大长度
	tfileinfo.lfname = little_malloc(SRAMIN, tfileinfo.lfsize);   //为长文件缓存区分配内存

	if ((res == FR_OK) && (tfileinfo.lfname != NULL))
	{
		while (1)                                           //查询总的有效文件数
		{
	        res = f_readdir(&tdir, &tfileinfo);       		//读取目录下的一个文件

            if ((res != FR_OK) || (tfileinfo.fname[0] == 0))    //错误了/到末尾了,退出		
            {
                break;	  
            }

            fn = (uint8_t *)(*tfileinfo.lfname ? tfileinfo.lfname : tfileinfo.fname);			 
			res = f_typetell(fn);	
			if ((res & 0xF0) == 0x50)                       //取高四位,看看是不是图片文件	
			{
				rval++;                                     //有效文件数增加1
			}	    
		}  
	} 

    f_closedir(&tdir);
    little_free(SRAMIN, tfileinfo.lfname);
	return rval;
}


void show_picture(void)
{
    uint8_t  res = 0;
    uint8_t  *fn;   			    //长文件名
    uint8_t  *p_full_name;			//带路径的文件名
    uint16_t picturenum;            //当前路径下图片总数
    uint16_t *p_idx_tbl;	        //图片索引表 
    uint16_t cur_idx;		        //图片当前索引
    uint16_t temp;
    DIR dir_pic;
    uint16_t width = g_tftlcd_lvgl_obj->tftlcd_cfg.p_dri->lcd_info.width;
    uint16_t height = g_tftlcd_lvgl_obj->tftlcd_cfg.p_dri->lcd_info.height;

    uint8_t *dir_path = "0:/picture";

    picturenum = pic_get_tnum(dir_path);
    if (picturenum == 0)
    {
        trace_error("no picture!\r\n");
        return ;
    }

    picfile_info.lfsize = _MAX_LFN * 2 + 1;                                 //长文件名最大长度
    picfile_info.lfname = little_malloc(SRAMIN, picfile_info.lfsize);       //为长文件缓存区分配内存
    p_full_name = little_malloc(SRAMIN, picfile_info.lfsize);               //为带路径的文件名分配内存
    p_idx_tbl = little_malloc(SRAMIN, 2 * picturenum);                      //申请2*totpicnum个字节的内存,用于存放图片索引

    while (picfile_info.lfname == NULL || p_full_name == NULL || p_idx_tbl == NULL)
    {
        trace_error("malloc fail.\r\n");
        return ; 
    }

    while (f_opendir(&dir_pic, dir_path))
    {
        trace_info("there's [error] in the [PICTURE] files.\r\n");
        // g_systick_obj.systick_ops.delay_ms(1000);
        return ;
    }

    cur_idx = 0;      
    while (1)           // 查询当前目录下的全部文件
    {
        temp = dir_pic.index;								
        res = f_readdir(&dir_pic, &picfile_info);       		
        if (res != FR_OK || picfile_info.fname[0] == 0)      	
        {
            break;	
        }  

        fn = (uint8_t *)(*picfile_info.lfname ? picfile_info.lfname : picfile_info.fname);			 
        res = f_typetell(fn);	
        if ((res & 0xF0) == 0x50)             
        {
            p_idx_tbl[cur_idx] = temp;      
            cur_idx++;
        }	    
    } 

    g_systick_obj.systick_ops.delay_ms(1500);
    piclib_init();
    cur_idx = 0;
    res = f_readdir(&dir_pic, &picfile_info);               // 使得dir_pic重新指向第一个文件

    while (res == FR_OK)
    {
		dir_sdi(&dir_pic, p_idx_tbl[cur_idx]);			    //改变当前目录索引	   
        res = f_readdir(&dir_pic, &picfile_info);       	//读取目录下的一个文件

        if (res != FR_OK || picfile_info.fname[0] == 0)	    //错误了/到末尾了,退出
        {
			break;	
        }

		fn = (uint8_t *)(*picfile_info.lfname ? picfile_info.lfname : picfile_info.fname);			 
		strcpy((char*)p_full_name, "0:/picture/");				//复制路径(目录)
		strcat((char*)p_full_name, (const char*)fn);  			//将文件名接在后面
        g_tftlcd_lvgl_obj->tftlcd_ops.clear_screen(&g_tftlcd_lvgl_obj->tftlcd_cfg, &g_tftlcd_lvgl_obj->tftlcd_ops);

		ai_load_picfile(p_full_name, 0, 0, width, height,1);	//显示图片  
        g_systick_obj.systick_ops.delay_ms(5000);          
    }

    little_free(SRAMIN, picfile_info.lfname);
    little_free(SRAMIN, p_full_name);
    little_free(SRAMIN, p_idx_tbl);
    f_closedir(&dir_pic);
}


