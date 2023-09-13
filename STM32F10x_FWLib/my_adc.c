#include "my_adc.h"


/*
ADC�ɼ�����
1��ADC_CO----------------------PB13
2����ص�ѹ--------------------PB14
*/

volatile u32t u32AdcCo	=	0;
volatile u32t u32AdcBAT	=	0;


///< ADC �����˿ڳ�ʼ��
void AdcPortInit(void)
{    
    ///< ����GPIO����ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    Gpio_SetAnalogMode(GpioPortB, GpioPin13);        //PB13 (AIN20)
    Gpio_SetAnalogMode(GpioPortB, GpioPin14);        //PB14 (AIN21)
}
///< ADC ģ���ʼ��
void AdcInit(void)
{
    stc_adc_cfg_t              stcAdcCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);	///< ����ADC/BGR����ʱ��
    
    Bgr_BgrEnable(); 																					///< ����BGR       
    
    ///< ADC ��ʼ������
    stcAdcCfg.enAdcMode         = AdcScanMode;								///<����ģʽ-ɨ��
    stcAdcCfg.enAdcClkDiv       = AdcMskClkDiv1;							///<������Ƶ-1
    stcAdcCfg.enAdcSampCycleSel = AdcMskSampCycle8Clk;				///<����������-8
    stcAdcCfg.enAdcRefVolSel    = AdcMskRefVolSelAVDD;				///<�ο���ѹѡ��-VCC
    stcAdcCfg.enAdcOpBuf        = AdcMskBufDisable;						///<OP BUF����-��
    stcAdcCfg.enInRef           = AdcMskInRefDisable;					///<�ڲ��ο���ѹʹ��-��
    stcAdcCfg.enAdcAlign        = AdcAlignRight;							///<ת��������뷽ʽ-��
	
    Adc_Init(&stcAdcCfg);
}

///< ADC ˳��ɨ�� ��������
void AdcSQRCfg(void)
{
    stc_adc_sqr_cfg_t          stcAdcSqrCfg;
    
    DDL_ZERO_STRUCT(stcAdcSqrCfg);
        
    ///< ˳��ɨ��ģʽ���ܼ�ͨ������
    ///< ע�⣺ɨ��ģʽ�£�������ת������Ϊnʱ��ת��ͨ�������÷�Χ����Ϊ[SQRCH(0)MUX,SQRCH(n-1)MUX]
    stcAdcSqrCfg.bSqrDmaTrig = FALSE;
    stcAdcSqrCfg.enResultAcc = AdcResultAccEnable;
    stcAdcSqrCfg.u8SqrCnt    = 2;
    Adc_SqrModeCfg(&stcAdcSqrCfg);

    Adc_CfgSqrChannel(AdcSQRCH0MUX, AdcExInputCH20);
    Adc_CfgSqrChannel(AdcSQRCH1MUX, AdcExInputCH21);
    
    ///< ADC �ж�ʹ��
//    Adc_EnableIrq();
//    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);
    
    ///< ����˳��ɨ�����
//    Adc_SQR_Start();

}

/////< ADC �жϷ������
//void Adc_IRQHandler(void)
//{    
//    if(TRUE == Adc_GetIrqStatus(AdcMskIrqSqr))
//    {
//        Adc_ClrIrqStatus(AdcMskIrqSqr);
//        
//        u32AdcRestult0   = Adc_GetSqrResult(AdcSQRCH0MUX);  //��ȡ˳��ɨ��ͨ��0
//        u32AdcRestult2   = Adc_GetSqrResult(AdcSQRCH1MUX);  //��ȡ˳��ɨ��ͨ��1
//        u32AdcRestult5   = Adc_GetSqrResult(AdcSQRCH2MUX);  //��ȡ˳��ɨ��ͨ��2
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
	
		AdcPortInit();		//����ADC��������Ϊģ������
		AdcInit();				//ADCģ������
		AdcSQRCfg();			//ADC˳��ɨ�蹦������
		delay1ms(500);
		Adc_SQR_Start();	///< ����˳��ɨ�����
		delay1ms(10);
	
		u32AdcCo	= Adc_GetSqrResult(AdcSQRCH0MUX);  //��ȡ˳��ɨ��ͨ��0
		u32AdcBAT	= Adc_GetSqrResult(AdcSQRCH1MUX);  //��ȡ˳��ɨ��ͨ��1
		
		real_v0 = u32AdcBAT*3.3/4095;
		real_v1 = u32AdcCo*3.3/4095;
	
		//����ʵ��ֵ
		*Bat = (real_v0/100)*143;
		*CO_DATA = real_v1*690;
	
		DEBUG_S("Li-Battery voltage = %6.4f v, CO ppm = %0d ppm.\r\n",*Bat,*CO_DATA);
		
		Adc_SQR_Stop();		///< �ر�˳��ɨ�����
		
		AdcBuf_Disable();	//�ر�ADC OP Buffer���͹���
		Bgr_BgrDisable();	//�ر�BGRģ�飬�͹���
    Adc_Disable(); 		//�ر�ADCģ�飬�͹���	
}


///< ExRef�˿ڳ�ʼ��
void App_ExrefPortInit(void)
{    
	stc_gpio_cfg_t stcGpioCfg;

	///< ��GPIO����ʱ���ſ�
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

	///< �˿ڷ�������->���
	stcGpioCfg.enDir = GpioDirOut;

	///< �˿�������������->����������
	stcGpioCfg.enDrv = GpioDrvL;

	///< �˿�����������->����
	stcGpioCfg.enPu = GpioPuDisable;//GpioPuEnable, GpioPuDisable

	///< �˿�����������->����
	stcGpioCfg.enPd = GpioPdDisable;//GpioPdEnable, GpioPdDisable

	///< �˿ڿ�©�������->��©/�������
	stcGpioCfg.enOD = GpioOdEnable;//GpioOdEnable, GpioOdDisable

	///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
	stcGpioCfg.enCtrlMode = GpioAHB;
		
	///< GPIO ��ʼ��
	Gpio_Init(GpioPortB, GpioPin1, &stcGpioCfg); 

	Gpio_WriteOutputIO(GpioPortB, GpioPin1, 0);
	
}

