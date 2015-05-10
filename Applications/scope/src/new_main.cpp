#include "new_main.h"

#include <DataManager.h>
#include <CustomParameters.h>
#include "../../rp_sdk/include/rpApp.h"
#include "../../rp_sdk/include/CustomParameters.h"


/***************************************************************************************
*                                     OSCILLOSCOPE                                     *
****************************************************************************************/

/* --------------------------------  OUT SIGNALS  ------------------------------ */
CFloatSignal ch1("ch1", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
CFloatSignal ch2("ch2", CH_SIGNAL_SIZE_DEFAULT, 0.0f);
CFloatSignal math("math", CH_SIGNAL_SIZE_DEFAULT, 0.0f);

/* ------------------------------- DATA PARAMETERS ------------------------------ */
CIntParameter dataSize("OSC_DATA_SIZE", CBaseParameter::RW, 1024, 0, 1, 16*1024);
CFloatParameter viewPosition("OSC_VIEW_POS", CBaseParameter::RO, 0.5, 0, 0, 1);
CFloatParameter viewPortion("OSC_VIEV_PART", CBaseParameter::RO, 0.1, 0, 0, 1);

/* --------------------------------  OUT PARAMETERS  ------------------------------ */
CBooleanParameter in1Show("CH1_SHOW", CBaseParameter::RW, false, 0);
CBooleanParameter in2Show("CH2_SHOW", CBaseParameter::RW, false, 0);

CBooleanParameter in1InvShow("CH1_SHOW_INVERTED", CBaseParameter::RW, true, 0);
CBooleanParameter in2InvShow("CH2_SHOW_INVERTED", CBaseParameter::RW, true, 0);

CBooleanParameter inReset("OSC_RST", CBaseParameter::RW, false, 0);
CBooleanParameter inRun("OSC_RUN", CBaseParameter::RW, false, 0);
CBooleanParameter inAutoscale("OSC_AUTOSCALE", CBaseParameter::RW, false, 0);
CBooleanParameter inSingle("OSC_SINGLE", CBaseParameter::RW, false, 0);

CFloatParameter in1Offset("OSC_CH1_OFFSET", CBaseParameter::RW, 0, 0, -40, 40);
CFloatParameter in2Offset("OSC_CH2_OFFSET", CBaseParameter::RW, 0, 0, -40, 40);
CFloatParameter inMathOffset("OSC_MATH_OFFSET", CBaseParameter::RW, 0, 0, -40, 40);
CFloatParameter in1Scale("OSC_CH1_SCALE", CBaseParameter::RW, 0.1, 0, 0, 1000);
CFloatParameter in2Scale("OSC_CH2_SCALE", CBaseParameter::RW, 0.1, 0, 0, 1000);
CFloatParameter inMathScale("OSC_MATH_SCALE", CBaseParameter::RW, 0.1, 0, 0, 1000);
CFloatParameter in1Probe("OSC_CH1_PROBE", CBaseParameter::RW, 10, 0, 0, 1000);
CFloatParameter in2Probe("OSC_CH2_PROBE", CBaseParameter::RW, 10, 0, 0, 1000);
CFloatParameter inTimeOffset("OSC_TIME_OFFSET", CBaseParameter::RW, 0, 0, -100000, 100000);
CFloatParameter inTimeScale("OSC_TIME_SCALE", CBaseParameter::RW, 1, 0, 0, 50000);

CCustomParameter<rpApp_osc_in_gain_t> in1Gain("OSC_CH1_IN_GAIN", CBaseParameter::RW, RPAPP_OSC_IN_GAIN_LV, 0, RPAPP_OSC_IN_GAIN_LV, RPAPP_OSC_IN_GAIN_HV);
CCustomParameter<rpApp_osc_in_gain_t> in2Gain("OSC_CH2_IN_GAIN", CBaseParameter::RW, RPAPP_OSC_IN_GAIN_LV, 0, RPAPP_OSC_IN_GAIN_LV, RPAPP_OSC_IN_GAIN_HV);

/* --------------------------------  TRIGGER PARAMETERS --------------------------- */
CFloatParameter inTriggLevel("OSC_TRIG_LEVEL", CBaseParameter::RW, 0, 0, -20, 20);
CCustomParameter<rpApp_osc_trig_sweep_t> inTrigSweep("OSC_TRIG_SWEEP", CBaseParameter::RW, RPAPP_OSC_TRIG_AUTO, 0, RPAPP_OSC_TRIG_AUTO, RPAPP_OSC_TRIG_SINGLE);
CCustomParameter<rpApp_osc_trig_source_t> inTrigSource("OSC_TRIG_SOURCE", CBaseParameter::RW, RPAPP_OSC_TRIG_SRC_CH1, 0, RPAPP_OSC_TRIG_SRC_CH1, RPAPP_OSC_TRIG_SRC_EXTERNAL);
CCustomParameter<rpApp_osc_trig_slope_t> inTrigSlope("OSC_TRIG_SLOPE", CBaseParameter::RW, RPAPP_OSC_TRIG_SLOPE_PE, 0, RPAPP_OSC_TRIG_SLOPE_NE, RPAPP_OSC_TRIG_SLOPE_PE);

/* --------------------------------  MEASURE  ------------------------------ */
CIntParameter measureSelect1("OSC_MEAS_SEL1", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect2("OSC_MEAS_SEL2", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect3("OSC_MEAS_SEL3", CBaseParameter::RW, -1, 0, -1, 23);
CIntParameter measureSelect4("OSC_MEAS_SEL4", CBaseParameter::RW, -1, 0, -1, 23);

CFloatParameter measureValue1("OSC_MEAS_VAL1", CBaseParameter::RW, 0, 0, -1000000, 1000000);
CFloatParameter measureValue2("OSC_MEAS_VAL2", CBaseParameter::RW, 0, 0, -1000000, 1000000);
CFloatParameter measureValue3("OSC_MEAS_VAL3", CBaseParameter::RW, 0, 0, -1000000, 1000000);
CFloatParameter measureValue4("OSC_MEAS_VAL4", CBaseParameter::RW, 0, 0, -1000000, 1000000);

/* --------------------------------  CURSORS  ------------------------------ */
CIntParameter cursor1("OSC_CURSOR1", CBaseParameter::RW, -1, 0, 0, 16*1024);
CIntParameter cursor2("OSC_CURSOR2", CBaseParameter::RW, -1, 0, 0, 16*1024);
CCustomParameter<rpApp_osc_source> cursorSrc("OSC_CURSOR_SRC", CBaseParameter::RW, RPAPP_OSC_SOUR_CH1, 0, RPAPP_OSC_SOUR_CH1, RPAPP_OSC_SOUR_MATH);

CFloatParameter cursor1V("OSC_CUR1_V", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor2V("OSC_CUR2_V", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor1T("OSC_CUR1_T", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursor2T("OSC_CUR2_T", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursorDT("OSC_CUR_DT", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursorDV("OSC_CUR_DV", CBaseParameter::RW, -1, 0, -1000, 1000);
CFloatParameter cursorDF("OSC_CUR_DF", CBaseParameter::RW, -1, 0, -1000, 1000);

/* ----------------------------------  MATH  -------------------------------- */
CCustomParameter<rpApp_osc_math_oper_t> mathOperation("OSC_MATH_OP", CBaseParameter::RW, RPAPP_OSC_MATH_NONE, 0, RPAPP_OSC_MATH_NONE, RPAPP_OSC_MATH_INT);
CCustomParameter<rp_channel_t> mathSource1("OSC_MATH_SRC1", CBaseParameter::RW, RP_CH_1, 0, RP_CH_1, RP_CH_2);
CCustomParameter<rp_channel_t> mathSource2("OSC_MATH_SRC2", CBaseParameter::RW, RP_CH_2, 0, RP_CH_1, RP_CH_2);





/***************************************************************************************
*                                      GENERATE                                        *
****************************************************************************************/

/* ------------------------------  OUT ARBITRARY SIGNAL ------------------------------ */
CFloatSignal arbitrarySignal("arbitrarySignal", 0, 0.0f);
CCustomParameter<rp_channel_t> arbitraryChannel("SOUR_TRAC_CH", CBaseParameter::RW, RP_CH_1, 0, RP_CH_1, RP_CH_2);

/* --------------------------------  OUTOUT PARAMETERS  ------------------------------ */
// TODO out1Show and out2Show unused
CBooleanParameter out1Show("OUTPUT1_SHOW", CBaseParameter::RW, true, 0);
CBooleanParameter out2Show("OUTPUT2_SHOW", CBaseParameter::RW, true, 0);

CBooleanParameter out1State("OUTPUT1_STATE", CBaseParameter::RW, false, 0);
CBooleanParameter out2State("OUTPUT2_STATE", CBaseParameter::RW, false, 0);
CFloatParameter out1Amplitude("SOUR1_VOLT", CBaseParameter::RW, 1, 0, -1, 1);
CFloatParameter out2Amplitude("SOUR2_VOLT", CBaseParameter::RW, 1, 0, -1, 1);
CFloatParameter out1Offset("SOUR1_VOLT_OFFS", CBaseParameter::RW, 0, 0, -1, 1);
CFloatParameter out2Offset("SOUR2_VOLT_OFFS", CBaseParameter::RW, 0, 0, -1, 1);
CFloatParameter out1Frequancy("SOUR1_FREQ_FIX", CBaseParameter::RW, 1000, 0, 0, 62.5e6);
CFloatParameter out2Frequancy("SOUR2_FREQ_FIX", CBaseParameter::RW, 1000, 0, 0, 62.5e6);

CFloatParameter out1Phase("SOUR1_PHAS", CBaseParameter::RW, 0, 0, -360, 360);
CFloatParameter out2Phase("SOUR2_PHAS", CBaseParameter::RW, 0, 0, -360, 360);
CFloatParameter out1DCYC("SOUR1_DCYC", CBaseParameter::RW, 0.5, 0, 0, 100);
CFloatParameter out2DCYC("SOUR2_DCYC", CBaseParameter::RW, 0.5, 0, 0, 100);

CCustomParameter<rp_waveform_t> out1WAveform("SOUR1_FUNC", CBaseParameter::RW, RP_WAVEFORM_SINE, 0, RP_WAVEFORM_SINE, RP_WAVEFORM_ARBITRARY);
CCustomParameter<rp_waveform_t> out2WAveform("SOUR2_FUNC", CBaseParameter::RW, RP_WAVEFORM_SINE, 0, RP_WAVEFORM_SINE, RP_WAVEFORM_ARBITRARY);

CBooleanParameter out1Burst("SOUR1_BURS_STAT", CBaseParameter::RW, false, 0);
CBooleanParameter out2Burst("SOUR2_BURS_STAT", CBaseParameter::RW, false, 0);

CCustomParameter<rp_trig_src_t> out1TriggerSource("SOUR1_TRIG_SOUR", CBaseParameter::RW, RP_GEN_TRIG_SRC_INTERNAL, 0, RP_GEN_TRIG_SRC_INTERNAL, RP_GEN_TRIG_GATED_BURST);
CCustomParameter<rp_trig_src_t> out2TriggerSource("SOUR2_TRIG_SOUR", CBaseParameter::RW, RP_GEN_TRIG_SRC_INTERNAL, 0, RP_GEN_TRIG_SRC_INTERNAL, RP_GEN_TRIG_GATED_BURST);


void UpdateParams(void) {
	if (measureSelect1.Value() != -1)
		measureValue1.Value() = getMeasureValue(measureSelect1.Value());
	if (measureSelect2.Value() != -1)
		measureValue2.Value() = getMeasureValue(measureSelect2.Value());
	if (measureSelect3.Value() != -1)
		measureValue3.Value() = getMeasureValue(measureSelect3.Value());
	if (measureSelect4.Value() != -1)
		measureValue4.Value() = getMeasureValue(measureSelect4.Value());

	if (cursor1.Value() != -1) {
		rpApp_OscGetCursorVoltage(cursorSrc.Value(), (uint32_t) cursor1.Value(), &cursor1V.Value());
		rpApp_OscGetCursorTime((uint32_t) cursor1.Value(), &cursor1T.Value());
	}
	if (cursor2.Value() != -1) {
		rpApp_OscGetCursorVoltage(cursorSrc.Value(), (uint32_t) cursor2.Value(), &cursor2V.Value());
		rpApp_OscGetCursorTime((uint32_t) cursor2.Value(), &cursor2T.Value());
	}
	if (cursor1.Value() != -1 && cursor1.Value() != -1) {
		rpApp_OscGetCursorDeltaAmplitude(cursorSrc.Value(), (uint32_t) cursor1.Value(), (uint32_t) cursor2.Value(), &cursorDV.Value());
		rpApp_OscGetCursorDeltaTime((uint32_t) cursor1.Value(), (uint32_t) cursor2.Value(), &cursorDT.Value());
		rpApp_OscGetCursorDeltaFrequency((uint32_t) cursor1.Value(), (uint32_t) cursor2.Value(), &cursorDF.Value());
	}

	float pos, portion;
	rpApp_OscGetViewPos(&pos);
	rpApp_OscGetViewPart(&portion);
	viewPosition.Value() = pos;
	viewPortion.Value() = portion;
}

float getMeasureValue(int measure) {
	float value;
	switch (measure) {
		case 0:
		case 1:
		case 2:
			rpApp_OscMeasureVpp((rpApp_osc_source) (measure % 3), &value);
			break;
		case 3:
		case 4:
		case 5:
			rpApp_OscMeasureAmplitudeMin((rpApp_osc_source) (measure % 3), &value);
			break;
		case 6:
		case 7:
		case 8:
			rpApp_OscMeasureAmplitudeMax((rpApp_osc_source) (measure % 3), &value);
			break;
		case 9:
		case 10:
		case 11:
			rpApp_OscMeasureMeanVoltage((rpApp_osc_source) (measure % 3), &value);
			break;
		case 12:
		case 13:
		case 14:
			rpApp_OscMeasurePeriod((rpApp_osc_source) (measure % 3), &value);
			break;
		case 15:
		case 16:
		case 17:
			rpApp_OscMeasureFrequency((rpApp_osc_source) (measure % 3), &value);
			break;
		case 18:
		case 19:
		case 20:
			rpApp_OscMeasureDutyCycle((rpApp_osc_source) (measure % 3), &value);
			break;
		case 21:
		case 22:
		case 23:
			rpApp_OscMeasureRootMeanSquare((rpApp_osc_source) (measure % 3), &value);
			break;
		default:
			value = 0;
	}
	return value;
}

void UpdateSignals(void) {
	float data[dataSize.Value()];
	if (in1Show.Value()) {
		rpApp_OscGetViewData(RPAPP_OSC_SOUR_CH1, data, (uint32_t) dataSize.Value());

		if (ch1.GetSize() != dataSize.Value())
			ch1.Resize(dataSize.Value());
		for (int i = 0; i < 1024; i++)
			ch1[i] = data[i];
	} else {
		ch1.Resize(0);
	}

	if (in2Show.Value()) {
		rpApp_OscGetViewData(RPAPP_OSC_SOUR_CH2, data, (uint32_t) dataSize.Value());

		if (ch2.GetSize() != dataSize.Value())
			ch2.Resize(dataSize.Value());
		for (int i = 0; i < 1024; i++)
			ch2[i] = data[i];
	} else {
		ch2.Resize(0);
	}

	if (mathOperation.Value() == RPAPP_OSC_MATH_NONE) {
		rpApp_OscGetViewData(RPAPP_OSC_SOUR_MATH, data, (uint32_t) dataSize.Value());

		if (math.GetSize() != dataSize.Value())
			math.Resize(dataSize.Value());
		for (int i = 0; i < 1024; i++)
			math[i] = data[i];
	} else {
		math.Resize(0);
	}
}

void OnNewParams(void) {
/* ------ UPDATE OSCILLOSCOPE LOCAL PARAMETERS ------*/
	in1Show.Update();
	in2Show.Update();
	dataSize.Update();
	measureSelect1.Update();
	measureSelect2.Update();
	measureSelect3.Update();
	measureSelect4.Update();

	cursor1.Update();
	cursor2.Update();
	cursorSrc.Update();

/* ------ SEND OSCILLOSCOPE PARAMETERS TO API ------*/
	if (inReset.NewValue()) {
		rpApp_OscReset();
	}
	if (inRun.NewValue() != inRun.Value()) {
		inRun.Update();
		if (inRun.Value()) {
			rpApp_OscRun();
		} else {
			rpApp_OscStop();
		}
	}

	if (inAutoscale.NewValue()) {
		rpApp_OscAutoScale();
		float value;
		rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH1, &value);
		in1Scale.Value() = value;
		rpApp_OscGetAmplitudeScale(RPAPP_OSC_SOUR_CH2, &value);
		in2Scale.Value() = value;
		rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH1, &value);
		in1Offset.Value() = value;
		rpApp_OscGetAmplitudeOffset(RPAPP_OSC_SOUR_CH2, &value);
		in1Offset.Value() = value;
		rpApp_OscGetTimeOffset(&value);
		inTimeOffset.Value() = value;
		rpApp_OscGetTimeScale(&value);
		inTimeScale.Value() = value;
	}
	if (inSingle.NewValue()) {
		rpApp_OscSingle();
	}

	IF_VALUE_CHANGED(in1Offset,    rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_CH1,  in1Offset.NewValue()))
	IF_VALUE_CHANGED(in2Offset,    rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_CH2,  in2Offset.NewValue()))
	IF_VALUE_CHANGED(inMathOffset, rpApp_OscSetAmplitudeOffset(RPAPP_OSC_SOUR_MATH, inMathOffset.NewValue()))
	IF_VALUE_CHANGED(in1Scale,    rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_CH1,  in1Scale.NewValue()))
	IF_VALUE_CHANGED(in2Scale,    rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_CH2,  in2Scale.NewValue()))
	IF_VALUE_CHANGED(inMathScale, rpApp_OscSetAmplitudeScale(RPAPP_OSC_SOUR_MATH, inMathScale.NewValue()))
	IF_VALUE_CHANGED(in1Probe, rpApp_OscSetProbeAtt(RP_CH_1, in1Probe.NewValue()))
	IF_VALUE_CHANGED(in2Probe, rpApp_OscSetProbeAtt(RP_CH_2, in2Probe.NewValue()))
	IF_VALUE_CHANGED(in1Gain, rpApp_OscSetInputGain(RP_CH_1, in1Gain.NewValue()))
	IF_VALUE_CHANGED(in2Gain, rpApp_OscSetInputGain(RP_CH_2, in2Gain.NewValue()))
	IF_VALUE_CHANGED(inTimeOffset, rpApp_OscSetTimeOffset(inTimeOffset.NewValue()))
	IF_VALUE_CHANGED(inTimeScale, rpApp_OscSetTimeScale(inTimeScale.NewValue()))
	IF_VALUE_CHANGED(inTrigSweep, rpApp_OscSetTriggerSweep(inTrigSweep.NewValue()))
	IF_VALUE_CHANGED(inTrigSource, rpApp_OscSetTriggerSource(inTrigSource.NewValue()))
	IF_VALUE_CHANGED(inTrigSlope, rpApp_OscSetTriggerSlope(inTrigSlope.NewValue()))
	IF_VALUE_CHANGED(inTriggLevel, rpApp_OscSetTriggerLevel(inTriggLevel.NewValue()))

	if (rpApp_OscSetMathSources(mathSource1.NewValue(), mathSource2.NewValue())) {
		mathSource1.Update();
		mathSource2.Update();
	}


/* ------ UPDATE GENERATE LOCAL PARAMETERS ------*/
	out1Show.Update();
	out2Show.Update();
	arbitraryChannel.Update();

/* ------ SEND GENERATE PARAMETERS RO API ------*/
	IF_VALUE_CHANGED(out1State, rp_GenAmp(RP_CH_1, out1State.NewValue()))
	IF_VALUE_CHANGED(out2State, rp_GenAmp(RP_CH_2, out2State.NewValue()))
	IF_VALUE_CHANGED(out1Offset, rp_GenOffset(RP_CH_1, out1Offset.NewValue()))
	IF_VALUE_CHANGED(out2Offset, rp_GenOffset(RP_CH_2, out2Offset.NewValue()))
	IF_VALUE_CHANGED(out1Frequancy, rp_GenFreq(RP_CH_1, out1Frequancy.NewValue()))
	IF_VALUE_CHANGED(out2Frequancy, rp_GenFreq(RP_CH_2, out2Frequancy.NewValue()))
	IF_VALUE_CHANGED(out1Phase, rp_GenPhase(RP_CH_1, out1Phase.NewValue()))
	IF_VALUE_CHANGED(out2Phase, rp_GenPhase(RP_CH_2, out2Phase.NewValue()))
	IF_VALUE_CHANGED(out1DCYC, rp_GenDutyCycle(RP_CH_1, out1DCYC.NewValue()))
	IF_VALUE_CHANGED(out2DCYC, rp_GenDutyCycle(RP_CH_2, out2DCYC.NewValue()))
	IF_VALUE_CHANGED(out1WAveform, rp_GenWaveform(RP_CH_1, out1WAveform.NewValue()))
	IF_VALUE_CHANGED(out2WAveform, rp_GenWaveform(RP_CH_2, out2WAveform.NewValue()))

	fprintf(stderr, "%d %f\n", out1WAveform.Value(), out2Frequancy.Value());

	IF_VALUE_CHANGED(out1Burst, rp_GenMode(RP_CH_1, out1Burst.NewValue() ? RP_GEN_MODE_BURST : RP_GEN_MODE_CONTINUOUS))
	IF_VALUE_CHANGED(out2Burst, rp_GenMode(RP_CH_2, out2Burst.NewValue() ? RP_GEN_MODE_BURST : RP_GEN_MODE_CONTINUOUS))
	IF_VALUE_CHANGED(out1TriggerSource, rp_GenTriggerSource(RP_CH_1, out1TriggerSource.NewValue()))
	IF_VALUE_CHANGED(out2TriggerSource, rp_GenTriggerSource(RP_CH_2, out2TriggerSource.NewValue()))
}