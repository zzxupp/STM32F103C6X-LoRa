#include "my_adc.h"


/*
ADC采集程序
1、ADC_CO----------------------PB13
2、电池电压--------------------PB14
*/

volatile u32t u32AdcCo	=	0;
volatile u32t u32AdcBAT	=	0;


///< ADC 采样端口初始化
void AdcPortInit(void)
{    
    ///< 开启GPIO外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    Gpio_SetAnalogMode(GpioPortB, GpioPin13);        //PB13 (AIN20)
    Gpio_SetAnalogMode(GpioPortB, GpioPin14);        //PB14 (AIN21)
}
///< ADC 模块初始化
void AdcInit(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);	///< 开启ADC/BGR外设时钟
    
    Bgr_BgrEnable(); 																					///< 开启BGR       
    
    ///< ADC 初始化配置
    stcAdcCfg.enAdcMode         = AdcScanMode;								///<采样模式-扫描
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv1;							///<采样分频-1
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk;				///<采样周期数-8
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;				///<参考电压选择-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;						///<OP BUF配置-关
    stcAdcCfg.enInRef           = AdcMskInRefDisable;					///<内部参考电压使能-关
    stcAdcCfg.enAdcAlign        = AdcAlignRight;							///<转换结果对齐方式-右
	
    Adc_Init(&stcAdcCfg);
}

///< ADC 顺序扫描 功能配置
void AdcSQRCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
        
    ///< 顺序扫描模式功能及通道配置
    ///< 注意：扫描模式下，当配置转换次数为n时，转换通道的配置范围必须为[SQRCH(0)MUX,SQRCH(n-1)MUX]
    stcAdcSqrCfg.bSqrDmaTrig = FALSE;
    stcAdcSqrCfg.enResultAcc = AdcResultAccEnable;
    stcAdcSqrCfg.u8SqrCnt    = 2;
    Adc_SqrModeCfg(&stcAdcSqrCfg);

    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH20);
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH21);
    
    ///< ADC 中断使能
//    Adc_EnableIrq();
//    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    ///< 启动顺序扫描采样
//    Adc_SQR_Start();

}

/////< ADC 中断服务程序
//void Adc_IRQHandler(void)
//{    
//    if(TRUE == Adc_GetIrqStatus(AdcMskIrqSqr))
//    {
//        Adc_ClrIrqStatus(AdcMskIrqSqr);
//        
//        u32AdcRestult0   = Adc_GetSqrResult(AdcSQRCH0MUX);  //获取顺序扫描通道0
//        u32AdcRestult2   = Adc_GetSqrResult(AdcSQRCH1MUX);  //获取顺序扫描通道1
//        u32AdcRestult5   = Adc_GetSqrResult(AdcSQRCH2MUX);  //获取顺序扫描通道2
//        
//        u32AdcRestultAcc = Adc_GetAccResult();
//        
//        Adc_SQR_Stop();
//    }
//}

void Adc_GetCOLibatResult(u16t *CO_DATA, ft *Bat)
{
		ft real_v0 = 0.0;
		ft real_v1 = 0.0;
	
		AdcPortInit();		//配置ADC输入引脚为模拟输入
		AdcInit();				//ADC模块配置
		AdcSQRCfg();			//ADC顺序扫描功能配置
		delay1ms(500);
		Adc_SQR_Start();	///< 启动顺序扫描采样
		delay1ms(10);
	
		u32AdcCo	= Adc_GetSqrResult(AdcSQRCH0MUX);  //获取顺序扫描通道0
		u32AdcBAT	= Adc_GetSqrResult(AdcSQRCH1MUX);  //获取顺序扫描通道1
		
		real_v0 = u32AdcBAT*3.3/4095;
		real_v1 = u32AdcCo*3.3/4095;
	
		//计算实际值
		*Bat = (real_v0/100)*143;
		*CO_DATA = real_v1*690;
	
		DEBUG_S("Li-Battery voltage = %6.4f v, CO ppm = %0d ppm.\r\n",*Bat,*CO_DATA);
		
		Adc_SQR_Stop();		///< 关闭顺序扫描采样
		
		AdcBuf_Disable();	//关闭ADC OP Buffer，低功耗
		Bgr_BgrDisable();	//关闭BGR模块，低功耗
    Adc_Disable(); 		//关闭ADC模块，低功耗	
}


///< ExRef端口初始化
void App_ExrefPortInit(void)
{    
	stc_gpio_cfg_t stcGpioCfg;

	///< 打开GPIO外设时钟门控
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	///< 端口方向配置->输出
	stcGpioCfg.enDir = GpioDirOut;

	///< 端口驱动能力配置->低驱动能力
	stcGpioCfg.enDrv = GpioDrvL;

	///< 端口上下拉配置->上拉
	stcGpioCfg.enPu = GpioPuDisable;//GpioPuEnable, GpioPuDisable

	///< 端口上下拉配置->下拉
	stcGpioCfg.enPd = GpioPdDisable;//GpioPdEnable, GpioPdDisable

	///< 端口开漏输出配置->开漏/推挽输出
	stcGpioCfg.enOD = GpioOdEnable;//GpioOdEnable, GpioOdDisable

	///< 端口输入/输出值寄存器总线控制模式配置->AHB
	stcGpioCfg.enCtrlMode = GpioAHB;
		
	///< GPIO 初始化
	Gpio_Init(GpioPortB, GpioPin1, &stcGpioCfg); 

	Gpio_WriteOutputIO(GpioPortB, GpioPin1, 0);
	
}

