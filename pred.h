#pragma once

class InputPrediction {
public:
	float m_curtime;
	float m_frametime;

public:
	struct viewmodel_t {
		float m_flViewmodelCycle;
		float m_flViewmodelAnimTime;
	} StoredViewmodel;

	void update( );
	void run( bool repredict );
	void restore( );

    void CorrectViewmodelData();

	void UpdateViewmodelData();
};

extern InputPrediction g_inputpred;