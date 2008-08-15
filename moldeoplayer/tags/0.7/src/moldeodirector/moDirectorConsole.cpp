#include "moDirectorConsole.h"
#include "moTextureFilterIndex.h"

BEGIN_EVENT_TABLE( moDirectorConsole, wxEvtHandler)
  EVT_TIMER( TICKS_ID, moDirectorConsole::OnTimer)
END_EVENT_TABLE()

moDirectorConsole::moDirectorConsole() {

	m_pDirectorCore = NULL;
	m_timerticks = 0;

}

moDirectorConsole::~moDirectorConsole() {

	m_timer.Stop();

}

MOboolean moDirectorConsole::Finish() {

	return moConsole::Finish();

}

void
moDirectorConsole::SetDirectorCore( moDirectorCore* p_pDirectorCore ) {

	m_pDirectorCore = p_pDirectorCore;
	SetNextActionHandler((moIDirectorActions*)m_pDirectorCore);
    m_timer.SetOwner( this, TICKS_ID);
	m_timer.Start(10);
}

moMoldeoObject*
moDirectorConsole::GetObject( moMobDescriptor p_MobDesc ) {

    moMoldeoObject* pMOB = NULL;

    MOint idx = p_MobDesc.GetMobDefinition().GetMobIndex().GetValueIndex();
    switch( p_MobDesc.GetMobDefinition().GetType() ) {
        case MO_OBJECT_PREEFFECT:
            if (idx>-1) pMOB = (moMoldeoObject*) m_EffectManager.PreEffects().Get(idx);
            break;
        case MO_OBJECT_EFFECT:
            if (idx>-1) pMOB = (moMoldeoObject*) m_EffectManager.Effects().Get(idx);
            break;
        case MO_OBJECT_POSTEFFECT:
            if (idx>-1) pMOB = (moMoldeoObject*) m_EffectManager.PostEffects().Get(idx);
            break;
        case MO_OBJECT_MASTEREFFECT:
            if (idx>-1) pMOB = (moMoldeoObject*) m_EffectManager.MasterEffects().Get(idx);
            break;
        case MO_OBJECT_IODEVICE:
            if (idx>-1) pMOB = (moMoldeoObject*) m_pIODeviceManager->IODevices().Get(idx);
            break;
        case MO_OBJECT_RESOURCE:
            if (idx>-1) {
                pMOB = (moMoldeoObject*) m_pResourceManager->GetResource(idx);
            } else {
                idx = m_pResourceManager->GetResourceId( p_MobDesc.GetMobDefinition().GetName() );
                pMOB = (moMoldeoObject*) m_pResourceManager->GetResource(idx);
            }
            break;

    }
    if (!pMOB)
        LogError( moText("moDirectorConsole::GetObject pMOB is NULL > idx = ") + IntToStr(idx));
    return pMOB;

}

//=========================================================================================================================
//NEW IDIRECTORACTIONS
//=========================================================================================================================


//================================================================
//Project
//================================================================


moDirectorStatus
moDirectorConsole::NewProject( moProjectDescriptor p_ProjectDescriptor )  {//create a console.cfg file with their effects.cfg

	//minimo: crear el archivo de consola .mol

	//crea las definiciones de par�metros
	GetDefinition();

	if ( m_Config.CreateDefault( p_ProjectDescriptor.GetFullConfigName() ) ) {
	    m_Config.GetParam( "devices" ).DeleteValue(0);
	    m_Config.GetParam( "resources" ).DeleteValue(0);
	    m_Config.GetParam( "preeffect" ).DeleteValue(0);
	    m_Config.GetParam( "effect" ).DeleteValue(0);
	    m_Config.GetParam( "posteffect" ).DeleteValue(0);
	    m_Config.GetParam( "mastereffect" ).DeleteValue(0);

	    if ( m_Config.SaveConfig( p_ProjectDescriptor.GetFullConfigName() ) == MO_CONFIG_OK ) {

            m_Config.UnloadConfig();

            Log( moText(".mol default project file created") );

            return OpenProject( p_ProjectDescriptor );
	    } else LogError( moText("Couldn't update mol file") );
    }

    m_pDirectorCore->LogError( moText("An error ocurred while creating mol file") );

    return MO_DIRECTOR_STATUS_ERROR;

}


moDirectorStatus
moDirectorConsole::OpenProject( moProjectDescriptor p_projectdescriptor )  {//load a console.cfg file with their effects.cfg
	//es importante reinicializar

	m_pResourceManager->Finish();

	if ( Initialized() ) {
		Finish();
	}

 	if( Init( p_projectdescriptor.GetConfigPath(),
              p_projectdescriptor.GetFullConfigName(),
              m_pIODeviceManager,
              m_pResourceManager,
              0,400,300,400,300,
              /*MO_DEF_SCREEN_WIDTH, MO_DEF_SCREEN_HEIGHT,
              MO_DEF_RENDER_WIDTH, MO_DEF_RENDER_HEIGHT,*/
              GetHandle())) {
        m_ProjectDescriptor = this->GetProject();

        //ahora actualizarmos el GUI
		m_pDirectorCore->ProjectUpdated( m_ProjectDescriptor );


        //========================================================================
        //  ACTUALIZAMOS TODOS LOS VALORES: para aquellos objetos que lo necesiten
        //========================================================================

        moMobDescriptors mMobs = GetMobDescriptors();

        for( int i=0; i < mMobs.Count(); i++ ) {

            moMobDescriptor mMob = mMobs[i];

            moParameterDescriptors  pParams = GetParameterDescriptors( mMob );

            if ( (mMob.GetMobDefinition().GetType()!=MO_OBJECT_IODEVICE) &&
                 (mMob.GetMobDefinition().GetType()!=MO_OBJECT_RESOURCE) &&
                 (mMob.GetMobDefinition().GetType()!=MO_OBJECT_MASTEREFFECT) ) {

                for( int j=0; j<pParams.Count(); j++ ) {

                    moParameterDescriptor pParam = pParams[j];

                    moValueIndex indx;
                    indx.m_ParamIndex = pParam.GetIndex();
                    indx.m_ValueIndex = pParam.GetIndexValue();

                    moValueDescriptor pValueDescriptor( pParam, indx );

                    pValueDescriptor = GetValue( pValueDescriptor );

                    ValueUpdated( pValueDescriptor );

                }

            }

        }

		//m_timer.Start();
        return MO_DIRECTOR_STATUS_OK;
	} else {
        LogError(moText("Couldn't init Console with current project"));
    }

	return MO_DIRECTOR_STATUS_ERROR;
}

moDirectorStatus
moDirectorConsole::CloseProject() {

	if ( Initialized() ) {
		Finish();
	}

    //m_timer.Stop();
	/*
	m_pDirectorCore->ProjectUpdated( m_ProjectDescriptor );
	*/


	return MO_DIRECTOR_STATUS_OK;
}

moDirectorStatus
moDirectorConsole::SaveProject() { //save a console.cfg file with their effects.cfg

    m_Config.SaveConfig();

	return MO_DIRECTOR_STATUS_OK;
}

moDirectorStatus
moDirectorConsole::SaveAsProject( moText p_configname, moText p_configpath ) { //save a console.cfg file with their effects.cfg

	return MO_DIRECTOR_STATUS_OK;
}

moDirectorStatus
moDirectorConsole::SetProject( moProjectDescriptor p_projectdescriptor ) {
	return MO_DIRECTOR_STATUS_OK;
}

moProjectDescriptor
moDirectorConsole::GetProject() {
    m_ProjectDescriptor = moProjectDescriptor( m_pResourceManager->GetDataMan()->GetConfigName(), m_pResourceManager->GetDataMan()->GetDataPath());
	return m_ProjectDescriptor;
}

moDirectorStatus
moDirectorConsole::ReloadProject() {

	return MO_DIRECTOR_STATUS_OK;
}


moMobDescriptors moDirectorConsole::GetMobDescriptors() {
    //generar la lista de MobDescriptor de todo el proyecto:
    moMobDescriptors    mMobDescriptors;

    for( int i=0;  i < (int) m_MoldeoObjects.Count(); i++) {
        moMoldeoObject* pMOB = m_MoldeoObjects.Get(i);
        if (pMOB) {
            moEffect* pEffect = NULL;
            switch(pMOB->GetType()) {
                case MO_OBJECT_IODEVICE:

                    break;
                case MO_OBJECT_RESOURCE:

                    break;
                default:
                    pEffect = (moEffect*) pMOB;
                    break;
            }

            moMobDescriptor mMobDescriptor( m_ProjectDescriptor, pMOB->GetMobDefinition() );

            if (pEffect)
                mMobDescriptor.SetState( moMobState(pEffect->state) );

            mMobDescriptors.Add( mMobDescriptor );
        }
    }

    return mMobDescriptors;
}


//================================================================
//Resourcec
//================================================================



    moResourceDescriptor moDirectorConsole::GetResourceDescriptor( moResourceDescriptor p_ResourceDescriptor ) {

        MOint result;
        moTexture* tex;

        if (!m_pResourceManager) {
            LogError(moText("Resource manager unavailable..."));
            return moResourceDescriptor();
        }

        switch( p_ResourceDescriptor.GetResourceDefinition().GetType() ) {
            case MO_RESOURCETYPE_TEXTURE:

                //moTextureDescriptor TexDescriptor( );

                result = m_pResourceManager->GetTextureMan()->GetTextureMOId( p_ResourceDescriptor.GetResourceDefinition().GetName(), false );

                if (result!=-1) {
                    tex = m_pResourceManager->GetTextureMan()->GetTexture( result );
                    if (tex) {

                       	moText filename = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)tex->GetName();

                        if ( moFile( filename ).Exists() ) {
                            //filename is ok
                            if (tex->IsBuildedFromFile()) {
                                //maybe a movie, lets doit
                                //maybe is a movie or you cant loadit
                                if ( tex->GetType()!=MO_TYPE_TEXTURE || p_ResourceDescriptor.IsCreateThumbnail() ) {
                                    //THEN CREATE THUMBNAIL
                                    filename = tex->CreateThumbnail( "PNG", 132, 88 );
                                }
                            }
                        } else {
                            //maybe is a movie or you cant loadit
                            if ( p_ResourceDescriptor.IsCreateThumbnail() ) {
                                //THEN CREATE THUMBNAIL
                                filename = tex->CreateThumbnail( moText("PNG"), 132, 88 );
                            }
                        }

                        moTextureDescriptor pTextDesc( tex->GetName(),
                                                       filename,
                                                       0,
                                                       (moTextureType)tex->GetType(),
                                                       m_pResourceManager->GetDataMan()->GetDataPath() );



                       pTextDesc.SetSpecificParameters( tex->GetWidth(),
                                                        tex->GetHeight(),
                                                        tex->GetComponents(),
                                                        tex->GetMOId(),
                                                        tex->GetGLId(),
                                                        tex->GetTexParam(),
                                                        tex->IsBuildedFromFile() );


                        return pTextDesc;
                    }

                }
                break;
            case MO_RESOURCETYPE_SHADER:
                break;
        }

        return moResourceDescriptor(); // return MO_RESOURCETYPE_UNDEFINED
    }

    moResourceDescriptors moDirectorConsole::GetResourceDescriptors(  moResourceType p_ResourceType  ) {

        moResourceDescriptors   pResourceDescriptors;

        moTextureManager* pTM = NULL;

        switch( p_ResourceType ) {
            case MO_RESOURCETYPE_TEXTURE:

                pTM = m_pResourceManager->GetTextureMan();

                if (pTM) {


                    moTexture*  tex;

                    for( int i = 0; i< pTM->GetTextureCount(); i++) {

                        tex = pTM->GetTexture(i);
                        if (tex) {
                            //ac� habr�a que diferenciar bien texturas pre-definidas de las cargadas por archivo...


                            moText filename = m_pResourceManager->GetDataMan()->GetDataPath() + moSlash + (moText)tex->GetName();
                            /*
                            if ( tex->GetType()!=MO_TYPE_TEXTURE ) {
                                //THEN CREATE THUMBNAIL
                                filename = tex->CreateThumbnail( moText("PNG"), 132, 88 );
                            }
                            */

                            moTextureDescriptor pTextDesc( tex->GetName(),
                                                           filename,
                                                           tex->GetWidth()*tex->GetHeight()*tex->GetComponents(), //just and idea of the size
                                                           (moTextureType)tex->GetType(),
                                                           m_pResourceManager->GetDataMan()->GetDataPath() );

                           pTextDesc.SetSpecificParameters( tex->GetWidth(),
                                                            tex->GetHeight(),
                                                            tex->GetComponents(),
                                                            tex->GetMOId(),
                                                            tex->GetGLId(),
                                                            tex->GetTexParam(),
                                                            tex->IsBuildedFromFile() );

                            pResourceDescriptors.Add( pTextDesc );

                            moResourceDescriptor testResourceDescriptor;

                            testResourceDescriptor = pResourceDescriptors[pResourceDescriptors.Count()-1];

                        }

                    }
                }

                break;

            case MO_RESOURCETYPE_FILTER:
                break;

            case MO_RESOURCETYPE_FONT:
                break;
        }

        return pResourceDescriptors;

    }


//================================================================
//MOB Moldeo Object
//================================================================

	moDirectorStatus
	moDirectorConsole::OpenMob( moMobDescriptor p_MobDesc ) {

        //chequeamos que el config este cargado

        MOint verif;
        moMoldeoObject* pMOB;

		pMOB = GetObject( p_MobDesc );

        if (pMOB) {
            if (pMOB->GetConfig()) {
                if (pMOB->GetConfig()->IsConfigLoaded()) {

                    return this->EditMob( p_MobDesc );

                } else LogError("moDirectorConsole::OpenMob Config file isn't loaded.");
            } else LogError("moDirectorConsole::OpenMob Config object unavailable");
        } else LogError(moText("moDirectorConsole::OpenMob Object not founded. Name: ") +
                        moText(p_MobDesc.GetMobDefinition().GetName()) +
                        moText(" Config Name:") +
                        moText(p_MobDesc.GetMobDefinition().GetConfigName()) +
                        moText(" Type:") +
                        moText( p_MobDesc.GetMobDefinition().GetTypeStr() )
                        );

		// todo: try to reload config file...

		return MO_DIRECTOR_STATUS_ERROR;

	}

	moDirectorStatus
	moDirectorConsole::CloseMob( moMobDescriptor p_MobDesc ) {
		/*
		if ( 0<=p_MobDesc.GetIndex() && p_MobDesc.GetIndex()< m_ProjectDescriptor.GetMobsCount(p_MobDesc.GetType())) {

			if( m_pMobs[p_MobDesc.GetType()][p_MobDesc.GetIndex()].IsConfigLoaded() ) {
				m_pMobs[p_MobDesc.GetType()][p_MobDesc.GetIndex()].UnloadConfig();
				return MO_DIRECTOR_STATUS_OK;
			} else {
				return MO_DIRECTOR_STATUS_ERROR;
			}
		} else
		*/
		return MO_DIRECTOR_STATUS_ERROR;
	}

	moDirectorStatus
	moDirectorConsole::ReloadMob( moMobDescriptor p_MobDesc ) {
		return MO_DIRECTOR_STATUS_OK;
	}

	moDirectorStatus
	moDirectorConsole::NewMob( moMobDescriptor p_MobDesc ) {

        moText confignamecomplete = "";
        bool res = false;

	    p_MobDesc.SetProjectDescriptor( GetProject() );

	    moMobDefinition& pMobDef( p_MobDesc.GetMobDefinition() );

	    moEffect* pEffect = NULL;
        moMoldeoObject* pMOB = NULL;
        moMobIndex& MobIndex( p_MobDesc.GetMobDefinition().GetMobIndex() );

	    switch(pMobDef.GetType()) {
	        case MO_OBJECT_PREEFFECT:
                MobIndex.SetParamIndex( m_Config.GetParamIndex("preeffect") );
                MobIndex.SetValueIndex( m_EffectManager.PreEffects().Count() );
                pMOB = (moMoldeoObject*)m_EffectManager.New( pMobDef );
                pEffect = (moEffect*) pMOB;
                if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "preeffect" );
                break;
            case MO_OBJECT_EFFECT:
                MobIndex.SetParamIndex( m_Config.GetParamIndex("effect") );
                MobIndex.SetValueIndex( m_EffectManager.Effects().Count() );
                pMOB = (moMoldeoObject*)m_EffectManager.New( pMobDef );
                pEffect = (moEffect*) pMOB;
                if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "effect" );
                break;
            case MO_OBJECT_POSTEFFECT:
                MobIndex.SetParamIndex( m_Config.GetParamIndex("posteffect") );
                MobIndex.SetValueIndex( m_EffectManager.PostEffects().Count() );
                pMOB = (moMoldeoObject*)m_EffectManager.New( pMobDef );
                pEffect = (moEffect*) pMOB;
                if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "posteffect" );
                break;
            case MO_OBJECT_IODEVICE:
                MobIndex.SetParamIndex( m_Config.GetParamIndex("iodevices") );
                MobIndex.SetValueIndex( m_pIODeviceManager->IODevices().Count() );
                pMOB = (moMoldeoObject*) m_pIODeviceManager->NewIODevice( pMobDef.GetName(), pMobDef.GetConfigName(), pMobDef.GetLabelName(), MO_OBJECT_IODEVICE, MobIndex.GetParamIndex(), MobIndex.GetValueIndex() );
                if (pMOB) pMOB->GetConfig()->Set( pMOB->GetName(), "iodevice" );
                break;
        }

        if (pMOB) {

            if (pMOB->GetConfigName().Length()>0) {

                confignamecomplete = m_pResourceManager->GetDataMan()->GetDataPath();
                confignamecomplete +=  moSlash + moText(pMOB->GetConfigName());
                confignamecomplete +=  moText(".cfg");

                pMOB->GetDefinition();

                res = pMOB->GetConfig()->CreateDefault( confignamecomplete );

                if (res) {
                    pMOB->GetConfig()->UnloadConfig();

                    //inicializacion....
                    m_MoldeoObjects.Add( (moMoldeoObject*) pMOB );

                    pMOB->SetResourceManager( m_pResourceManager );

                    res = pMOB->Init();
                    if (res) {
                        if (pEffect) {
                            pEffect->LoadCodes( m_pIODeviceManager );
                            pEffect->state.on = MO_ON;
                        }
                    } else {
                        LogError( moText("moDirectorConsole::NewMob Couldn't initialized effect" ) );
                    }

                    //pEffect->Draw( &state.tempo );
                    moValue effectvalue( pMOB->GetName(), "TXT" );
                    effectvalue.AddSubValue( pMOB->GetConfigName() , "TXT" );
                    effectvalue.AddSubValue( pMOB->GetLabelName() , "TXT" );
                    effectvalue.AddSubValue( "0" , "NUM" );
                    effectvalue.AddSubValue( "1" , "NUM" );

                    m_Config.GetParam( pMOB->GetMobDefinition().GetMobIndex().GetParamIndex() ).AddValue( effectvalue );

                    //LoadConnections();

                    ProjectUpdated( m_ProjectDescriptor );

                } else {
                    LogError( moText("moDirectorConsole::NewMob Couldn't create default configuration") );
                }

            } else {
                LogError( moText("moDirectorConsole::NewMob Config filename undefined" ) );
            }
        } else {
            LogError( moText("moDirectorConsole::NewMob Couldn't create effect" ) );
        }

        return MO_DIRECTOR_STATUS_ERROR;

	}

	moDirectorStatus
	moDirectorConsole::EditMob( moMobDescriptor p_MobDesc ) {

        moDirectorStatus res;

        res = m_pDirectorCore->EditMob(p_MobDesc);

        if (res!=MO_DIRECTOR_STATUS_OK) {
            LogError( moText("moDirectorConsole::EditMob Couldn't edit ")+(moText)p_MobDesc.GetMobDefinition().GetConfigName());
            return MO_DIRECTOR_STATUS_ERROR;
        }


        moMoldeoObject* pMOB = NULL;
        moConfig* pConfig = NULL;

		pMOB = GetObject( p_MobDesc );
        if (pMOB)
            pConfig = pMOB->GetConfig();
        else
            return MO_DIRECTOR_STATUS_ERROR;

        if (pConfig) {
            for( int i=0; i<pConfig->GetParamsCount(); i++ ) {

                moParam& Param( pConfig->GetParam(i));

                ///OJO!!! esto deberia estar en la inicializacion del config
                Param.GetParamDefinition().SetIndex(i);

                moParameterDescriptor ParamDescriptor( p_MobDesc, Param.GetParamDefinition(), i, Param.GetIndexValue() );

                //create control
                this->EditParameter( ParamDescriptor );


                for( MOuint j=0; j<pConfig->GetParam(i).GetValuesCount(); j++) {

                    moValueIndex Vindex;

                    Vindex.m_ParamIndex = i;
                    Vindex.m_ValueIndex = j;

                    moValueDescriptor ValueDescriptor( ParamDescriptor, Vindex );
                    ValueDescriptor.SetValue(pConfig->GetParam(i).GetValue(j));

                    this->EditValue( ValueDescriptor );

                }

                //preconf ???
                ParameterUpdated( ParamDescriptor );

            }

            return MO_DIRECTOR_STATUS_OK;
        }

		return MO_DIRECTOR_STATUS_ERROR;
	}

	moDirectorStatus
	moDirectorConsole::SaveMob( moMobDescriptor p_MobDesc ) {

        moMoldeoObject* pMOB = NULL;

		MOint idx = p_MobDesc.GetMobDefinition().GetMobIndex().GetValueIndex();

		//if ( 0<=idx && idx < (int)m_EffectManager.Effects.Count() ) {
		//for ( int idx=0; idx < (int)m_MoldeoObjects.Count(); idx++ ) {
        switch( p_MobDesc.GetMobDefinition().GetType() ) {
            case MO_OBJECT_PREEFFECT:
                pMOB = (moMoldeoObject*) m_EffectManager.PreEffects().Get(idx);
                break;
            case MO_OBJECT_EFFECT:
                pMOB = (moMoldeoObject*) m_EffectManager.Effects().Get(idx);
                break;
            case MO_OBJECT_POSTEFFECT:
                pMOB = (moMoldeoObject*) m_EffectManager.PostEffects().Get(idx);
                break;
        }

		if (pMOB && (pMOB->GetConfig()->SaveConfig()!=MO_CONFIG_OK) ) {
            return MO_DIRECTOR_STATUS_ERROR;
        } else return MO_DIRECTOR_STATUS_OK;
	}

	moDirectorStatus
	moDirectorConsole::DeleteMob( moMobDescriptor p_MobDesc ) {

	    moMoldeoObject* pMOB;

		pMOB = GetObject( p_MobDesc );

		bool consoleinit = m_bInitialized;

        if (pMOB) {

            if ( pMOB->GetId() >= MO_MOLDEOOBJECTS_OFFSET_ID ) {

                //just wait until we delete the object
                //important to avoid conflicts on loop...
                //maybe MUST BE A LOCK
                m_bInitialized = false;


                //remove from MoldeoObjects object
                m_MoldeoObjects.Remove( pMOB->GetId() - MO_MOLDEOOBJECTS_OFFSET_ID );

                //reload connections and id's
                LoadConnections();

                MOint idx = p_MobDesc.GetMobDefinition().GetMobIndex().GetValueIndex();
                if (idx>-1) {
                    switch( pMOB->GetType() ) {
                        case MO_OBJECT_PREEFFECT:
                        case MO_OBJECT_EFFECT:
                        case MO_OBJECT_POSTEFFECT:
                            m_EffectManager.RemoveEffect( idx, pMOB->GetType());
                            break;
                    }
                }

                //back to previous state
                m_bInitialized = consoleinit;

                //we've finished, update project
                ProjectUpdated( GetProject() );

            }
        }


		return MO_DIRECTOR_STATUS_OK;
	}

	moMobDescriptor
	moDirectorConsole::GetMob( moMobDescriptor p_MobDesc ) {

        moMoldeoObject* pMOB;

		pMOB = GetObject( p_MobDesc );

        if (pMOB) {
            /// TODO ya no deberia ser necesario pasar el puntero...esto se puede sacar

            switch(pMOB->GetType()) {
                case MO_OBJECT_IODEVICE:
                case MO_OBJECT_RESOURCE:
                    break;
                default:
                    moEffect* pEffect = (moEffect*) pMOB;
                    p_MobDesc.SetState( moMobState(pEffect->state) );
                    break;
            }
            return p_MobDesc;
        }

		return p_MobDesc; //sin modificaciones
	}

	moDirectorStatus
	moDirectorConsole::SetMob( moMobDescriptor p_MobDesc ) {

		moMoldeoObject* pMOB = GetObject( p_MobDesc );

        if (pMOB) {

            moEffect* Effect = (moEffect*) pMOB;

            Effect->state.on = p_MobDesc.GetState().GetEffectState().on;
            Effect->state.alpha = p_MobDesc.GetState().GetEffectState().alpha;
            Effect->state.tint = p_MobDesc.GetState().GetEffectState().tint;
            Effect->state.tempo.delta = p_MobDesc.GetState().GetEffectState().tempo.delta;
            Effect->state.synchronized= p_MobDesc.GetState().GetEffectState().synchronized;
            Effect->state.CSV2RGB();
            return MO_DIRECTOR_STATUS_OK;
        }

		return MO_DIRECTOR_STATUS_ERROR;
	}

	moDirectorStatus
	moDirectorConsole::AddMobToProject( moMobDescriptor p_MobDesc ) {
		return MO_DIRECTOR_STATUS_OK;
	}

	moDirectorStatus
	moDirectorConsole::RemoveMobToProject( moMobDescriptor p_MobDesc ) {
		return MO_DIRECTOR_STATUS_OK;
	}

	moDirectorStatus
	moDirectorConsole::MoveMobInProject( moMobDescriptor p_MobDesc ) {
		return MO_DIRECTOR_STATUS_OK;
	}

	moParameterDescriptors  moDirectorConsole::GetParameterDescriptors( moMobDescriptor p_MobDesc) {

        moParameterDescriptors    mParameterDescriptors;
        moMoldeoObject* pMOB = GetObject( p_MobDesc );

        if (pMOB) {

            moConfig* pConfig = pMOB->GetConfig();

            if ( pConfig ) {
                if (pConfig->IsConfigLoaded() ) {
                    for(int i=0; i < pConfig->GetParamsCount(); i++) {

                        moParam& mParam( pConfig->GetParam(i) );

                        moParamDefinition mParamDefinition( mParam.GetParamDefinition() );
                        moParameterDescriptor   mParamDescriptor( p_MobDesc, mParamDefinition, i, mParam.GetIndexValue() );

                        mParameterDescriptors.Add( mParamDescriptor );
                    }
                }
            }

        }

	    return mParameterDescriptors;
    }


moDirectorStatus
moDirectorConsole::SetParameter( moParameterDescriptor  p_ParameterDesc ) {

    moMoldeoObject* pMOB = GetObject( p_ParameterDesc.GetMobDescriptor() );
    if (pMOB) {

        moConfig* pConfig = NULL;

        pConfig = pMOB->GetConfig();
        if (pConfig) {
            moParam& Param( pConfig->GetParam( p_ParameterDesc.GetIndex() ) );

            Param.SetIndexValue( p_ParameterDesc.GetIndexValue() );

            ParameterUpdated( p_ParameterDesc );

            return MO_DIRECTOR_STATUS_OK;
        }

    }
    return MO_DIRECTOR_STATUS_ERROR;
}

//=========================================================================================================================
//
//=========================================================================================================================

    moValueDescriptor moDirectorConsole::GetValue( moValueDescriptor p_ValueDesc ) {

        moConfig* pConfig;
        moMoldeoObject* pObject;


        pObject = GetObject( p_ValueDesc.GetParamDescriptor().GetMobDescriptor() );
        if (pObject) {

             pConfig = pObject->GetConfig();

            if (pConfig) {

                moParamDefinition ParamDef = p_ValueDesc.GetParamDescriptor().GetParamDefinition();

                moParam& Param( pConfig->GetParam( ParamDef.GetIndex() ) );

                moValue& Value( Param.GetValue( p_ValueDesc.GetValueIndex().m_ValueIndex ) );

                p_ValueDesc.SetValue( Value );
            }
        }

        return p_ValueDesc;
    }

	moDirectorStatus moDirectorConsole::InsertValue( moValueDescriptor p_ValueDesc ) {

        //insert value...
        //at the end...
        moConfig* pConfig;
        moMoldeoObject* pObject;


        pObject = GetObject( p_ValueDesc.GetParamDescriptor().GetMobDescriptor() );
        pConfig = pObject->GetConfig();

        moParam& Param( pConfig->GetParam( p_ValueDesc.GetParamDescriptor().GetIndex() ) );

        Param.AddValue( p_ValueDesc.GetValue() );

        int newindexvalue = Param.GetValuesCount() - 1;

        p_ValueDesc.GetValueDefinition().SetIndex( newindexvalue );
        p_ValueDesc.GetValueIndex().m_ValueIndex =  newindexvalue;

        EditValue( p_ValueDesc );

        p_ValueDesc.GetParamDescriptor().SetIndexValue(newindexvalue);

        Param.SetIndexValue( newindexvalue );
        ParameterUpdated( p_ValueDesc.GetParamDescriptor() );

	    return MO_DIRECTOR_STATUS_OK;
    }

    moDirectorStatus moDirectorConsole::DeleteValue( moValueDescriptor p_ValueDesc ) {

        moConfig* pConfig;
        moMoldeoObject* pObject;


        pObject = GetObject( p_ValueDesc.GetParamDescriptor().GetMobDescriptor() );
        pConfig = pObject->GetConfig();

        moParam& Param( pConfig->GetParam( p_ValueDesc.GetParamDescriptor().GetIndex() ) );

        if ( Param.GetValuesCount() > 1 ) {
            Param.DeleteValue( p_ValueDesc.GetValueIndex().m_ValueIndex );
        } else return MO_DIRECTOR_STATUS_ERROR;

        return MO_DIRECTOR_STATUS_OK;
    }

	moDirectorStatus moDirectorConsole::SaveValue( moValueDescriptor p_ValueDesc ) {

        //save value in config class


	    return MO_DIRECTOR_STATUS_OK;
    }

    moDirectorStatus moDirectorConsole::SetValue( moValueDescriptor p_ValueDesc ) {

        //aqui hay q tratar correctamente los errores...

        int idx;
        moFont* pFont;

        //fija el valor dentro del config y chequea de actualizar el MOB
        moConfig* pConfig;
        moMoldeoObject* pObject;
        bool firsthaschange = false;
        bool secondhaschange = false;
        bool thirdhaschange = false;


        pObject = GetObject( p_ValueDesc.GetParamDescriptor().GetMobDescriptor() );
        if (pObject) {
            pConfig = pObject->GetConfig();

            if (pConfig) {
                moParamDefinition ParamDef = p_ValueDesc.GetParamDescriptor().GetParamDefinition();
                moParam& Param( pConfig->GetParam( ParamDef.GetIndex() ) );
                moValue& Value( Param.GetValue( p_ValueDesc.GetValueIndex().m_ValueIndex ) );


                //=============================================
                //COMPARAMOS CON EL ANTERIOR (JUST FOR SHADERS)
                //=============================================

                firsthaschange = !( Value.GetSubValue(0).Text() == p_ValueDesc.GetValue().GetSubValue(0).Text() );

                if ( Value.GetSubValueCount() >= 1 ) {
                    secondhaschange = !( Value.GetSubValue(1).Text() == p_ValueDesc.GetValue().GetSubValue(1).Text() );
                }
                if ( Value.GetSubValueCount() >= 2 ) {
                    thirdhaschange = !( Value.GetSubValue(2).Text() == p_ValueDesc.GetValue().GetSubValue(2).Text() );
                }

                //asigna el nuevo valor al config
                Value = p_ValueDesc.GetValue();

                //ahora segun el tipo de parametro se procesa el valor...
                switch(ParamDef.GetType()) {
                    case MO_PARAM_ALPHA:
                    case MO_PARAM_FUNCTION:
                    case MO_PARAM_TRANSLATEX:
                    case MO_PARAM_TRANSLATEY:
                    case MO_PARAM_TRANSLATEZ:
                    case MO_PARAM_SCALEX:
                    case MO_PARAM_SCALEY:
                    case MO_PARAM_SCALEZ:
                    case MO_PARAM_ROTATEX:
                    case MO_PARAM_ROTATEY:
                    case MO_PARAM_ROTATEZ:
                    case MO_PARAM_SYNC:
                    case MO_PARAM_PHASE:
                        idx = m_pResourceManager->GetMathMan()->AddFunction( Value.GetSubValue(0).Text(), (MOboolean)false );

                        if (idx>-1)
                            Value.GetSubValue(0).SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );

                        break;

                    case MO_PARAM_NUMERIC:
                        /*char* vtext = (char*)p_ValueDesc.GetValue().GetSubValue().Text();
                        MOlong ivalue = strtol( "%i", &vtext, 10 );
                        Value.GetSubValue(0).SetInt( ivalue );
                        */
                        Param.GetValue(p_ValueDesc.GetValueIndex().m_ValueIndex) = p_ValueDesc.GetValue();
                        break;

                    case MO_PARAM_TEXT:
                        Param.GetValue(p_ValueDesc.GetValueIndex().m_ValueIndex) = p_ValueDesc.GetValue();
                        break;
                    case MO_PARAM_3DMODEL:
                    case MO_PARAM_OBJECT:
                        /*
                        mo3dModel* pModel = m_pResourceManager->GetModelMan()->Get3dModel( Value.GetSubValue(0).Text() );
                        mo3DModel* p3DModel = new mo3dModel(mo3dsModel);

                        if (pModel)
                            Value.GetSubValue(0).SetModel( (mo3DModel*)pModel );
                            */
                        break;

                    case MO_PARAM_FONT:
                        pFont = m_pResourceManager->GetFontMan()->AddFont( Value.GetSubValue(0).Text() );
                        if (pFont)
                            Value.GetSubValue(0).SetFont( pFont );
                        break;

                    case MO_PARAM_TEXTURE:
                    case MO_PARAM_VIDEO:
                        /// TODO: reescribir teniendo en cuenta si tiene o no aplicado un shader...
                        if ( Param.GetParamDefinition().GetType() == MO_PARAM_TEXTURE
                            || Param.GetParamDefinition().GetType() == MO_PARAM_VIDEO ) {

                            moValueBase& valuebase( Value.GetSubValue(0) );

                            //=========================================
                            // FIRST SUBVALUE CORRESPOND TO FILE IMAGE;
                            // IF IT CHANGES WE MUST RELOAD ALL....
                            //=========================================
                            if ( firsthaschange ) {

                                idx = m_pResourceManager->GetTextureMan()->GetTextureMOId( valuebase.Text(), true);

                                if (idx>-1) {
                                    moTexture*  pTexture = m_pResourceManager->GetTextureMan()->GetTexture(idx);
                                    valuebase.SetTexture( pTexture );
                                } else {
                                    LogError( moText("Param ") + Param.GetParamDefinition().GetName() +  moText(" Texture undefined") );
                                }
                            }

                            //=====================================================
                            // SECOND SUBVALUE CORRESPOND TO SHADER CONFIG
                            // IF firstchange HAS TRIGGREER THIS STEP IS MANDATORY
                            //=====================================================
                            if  ( Value.GetSubValueCount()>1 ) {

                                if ( firsthaschange || secondhaschange || thirdhaschange ) {
                                    //============================
                                    //RELOAD TEXTUREFILTER
                                    //============================
                                    idx = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->TextureFilterExists( &Value );

                                    if (idx==-1) {
                                        idx = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->LoadFilter( &Value );

                                        if ( (idx-1)>=0 && (idx-1) < m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->Count() ) {
                                            moTextureFilter*  pTextureFilter = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->Get(idx-1);
                                            valuebase.SetTextureFilter( pTextureFilter );
                                        } else {
                                            LogError( moText("Param ") + Param.GetParamDefinition().GetName() +  moText(" Texture filter undefined") );
                                        }
                                    } else {
                                        moText tfname = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->MakeTextureFilterLabelName( &Value );
                                        Log( moText("Param ") + Param.GetParamDefinition().GetName() +  moText(" Texture filter already defined: ") + (moText)tfname );
                                        moTextureFilter*  pTextureFilter = m_pResourceManager->GetShaderMan()->GetTextureFilterIndex()->Get(idx);
                                        valuebase.SetTextureFilter( pTextureFilter );
                                    }


                                }
                            }

                            //=====================================================
                            // NOW TRY TO ADJUST ALPHA FILTER IF PARAMETER EXISTS
                            //=====================================================

                            if (Value.GetSubValueCount()>=4) {

                                //======================================
                                //ALPHA FILTER
                                //======================================

                                moValueBase& VAlpha( Value.GetSubValue(3) );


                                if (VAlpha.GetType()==MO_VALUE_FUNCTION) {

                                    idx = m_pResourceManager->GetMathMan()->AddFunction( VAlpha.Text(), (MOboolean)false );
                                    if (idx>-1)
                                        VAlpha.SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );

                                }
                                valuebase.SetTextureFilterAlpha( VAlpha.GetData() );

                            }


                            //======================================
                            //TEXTURE FILTER PARAMETERS
                            //======================================

                            if (Value.GetSubValueCount()>4) {
                                //tenemos que usar los filtros-param
                                moTextFilterParam*  pFilterParam = new moTextFilterParam();

                                for( int i=4; i<Value.GetSubValueCount(); i++) {
                                    //si o si tenemos que usar los codes... o names para los subvalores...
                                    moValueBase& vbase( Value.GetSubValue(i) );

                                    if ( vbase.GetCodeName() == moText("float1") ) {
                                        if ( vbase.Type() == MO_DATA_NUMBER_FLOAT ) {
                                            pFilterParam->par_flt1 = vbase.Float();
                                        }
                                    } else
                                    if ( vbase.GetCodeName() == moText("float2") ) {
                                        if ( vbase.Type() == MO_DATA_NUMBER_FLOAT ) {
                                            pFilterParam->par_flt2 = vbase.Float();
                                        }
                                    } else
                                    if ( vbase.GetCodeName() == moText("float3") ) {
                                        if ( vbase.Type() == MO_DATA_NUMBER_FLOAT ) {
                                            pFilterParam->par_flt3 = vbase.Float();
                                        }
                                    } else
                                    if ( vbase.GetCodeName() == moText("mat2") ) {
                                        if ( vbase.Type() == MO_DATA_VECTOR && vbase.Size() == 4 ) {
                                            float* vector = (float*)vbase.Pointer();
                                            for( int i=0; i<4 , pFilterParam->par_mat2[i] = vector[i] ; i++);
                                        }
                                    } else
                                    if ( vbase.GetCodeName() == moText("mat3") ) {
                                        if ( vbase.Type() == MO_DATA_VECTOR && vbase.Size() == 9 ) {
                                            float* vector = (float*)vbase.Pointer();
                                            for( int i=0; i<9 , pFilterParam->par_mat3[i] = vector[i] ; i++);
                                        }
                                    } else
                                    if ( vbase.GetCodeName() == moText("mat4") ) {
                                        if ( vbase.Type() == MO_DATA_VECTOR && vbase.Size() == 16 ) {
                                            float* vector = (float*)vbase.Pointer();
                                            for( int i=0; i<16 , pFilterParam->par_mat4[i] = vector[i] ; i++);
                                        }
                                    }
                                }
                                valuebase.SetTextureFilterParam( pFilterParam );
                            }

                        }
                        break;
                    case MO_PARAM_FILTER:
                        //Value.GetSubValue(0).SetText(  );
                        break;

                    case MO_PARAM_BLENDING:
                        Param.GetValue(p_ValueDesc.GetValueIndex().m_ValueIndex) = p_ValueDesc.GetValue();
                        break;

                    case MO_PARAM_COLOR:
                        //RED=============================================================================
                        idx = m_pResourceManager->GetMathMan()->AddFunction( Value.GetSubValue(MO_RED).Text(), (MOboolean)false );
                        if (idx>-1)
                            Value.GetSubValue(MO_RED).SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );
                        //GREEN=============================================================================
                        idx = m_pResourceManager->GetMathMan()->AddFunction( Value.GetSubValue(MO_GREEN).Text(), (MOboolean)false );
                        if (idx>-1)
                            Value.GetSubValue(MO_GREEN).SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );
                        //BLUE=============================================================================
                        idx = m_pResourceManager->GetMathMan()->AddFunction( Value.GetSubValue(MO_BLUE).Text(), (MOboolean)false );
                        if (idx>-1)
                            Value.GetSubValue(MO_BLUE).SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );
                        //ALPHA=============================================================================
                        idx = m_pResourceManager->GetMathMan()->AddFunction( Value.GetSubValue(MO_ALPHA).Text(), (MOboolean)false );
                        if (idx>-1)
                            Value.GetSubValue(MO_ALPHA).SetFun( m_pResourceManager->GetMathMan()->GetFunction(idx) );
                        break;

                    default:
                        Param.GetValue(p_ValueDesc.GetValueIndex().m_ValueIndex) = p_ValueDesc.GetValue();
                        break;
                };

                //para aquellos datos que implican cambios, lo fijamos de nuevo...
                    // funciones, texturas filtradas, objetos, fuentes, etc...
                p_ValueDesc.SetValue( Value );

                //notificamos todos los modulos de la actualizacion de estos datos
                ValueUpdated( p_ValueDesc );
            } //config
        } //object

	    return MO_DIRECTOR_STATUS_OK;
    }


moValueDescriptors  moDirectorConsole::GetValueDescriptors( moParameterDescriptor p_ParamDesc ) {

    moValueDescriptors mValues;

    moConfig* pConfig;
    moMoldeoObject* pObject;


    pObject = GetObject( p_ParamDesc.GetMobDescriptor() );
    if (pObject) {

        pConfig = pObject->GetConfig();

        if (pConfig) {

            moParamDefinition ParamDef = p_ParamDesc.GetParamDefinition();

            moParam& Param( pConfig->GetParam( ParamDef.GetIndex() ) );

            for( int i=0; i< Param.GetValuesCount(); i++ ) {

                moValue& Value( Param.GetValue( i ) );

                moValueIndex ix;
                ix.m_ParamIndex = Param.GetParamDefinition().GetIndex();
                ix.m_ValueIndex = i;

                mValues.Add( moValueDescriptor(  p_ParamDesc, ix ) );

            }
        }
    }


    return mValues;
}


void
moDirectorConsole::OnTimer( wxTimerEvent &event) {

	m_timerticks+=10;
	ConsoleLoop();

}


void
moDirectorConsole::Draw() { //refresh actual console.cfg file with their effects.cfg

	if( Initialized() ) {
		moConsole::Draw();
	}

}

MOulong
moDirectorConsole::GetTicks() {

	//return(MOulong)::wxGetElapsedTime();
	return m_timerticks;

}

void
moDirectorConsole::GLSwapBuffers() {

    if (m_pDirectorCore)
        m_pDirectorCore->ViewSwapBuffers();

}

moDirectorStatus
moDirectorConsole::SetView( int x, int y, int w, int h ) {

    Log( moText("moDirectorConsole::SetView Changing View: ") + IntToStr(w) + moText("X") + IntToStr(h) );

    if (Initialized()) {
        Log( moText("moDirectorConsole::SetView WITH CONSOLE") );
        if (m_pResourceManager)
            if (m_pResourceManager->Initialized()) {
            		if (m_pResourceManager->GetRenderMan()) {
						m_pResourceManager->GetRenderMan()->Finish();
						m_pResourceManager->GetRenderMan()->Init( 0, w, h, w, h);
						Log("moDirectorConsole::SetView Render Manager Resized");
					}

            }
    } else {
        Log( moText("moDirectorConsole::SetView NO CONSOLE") );

        glViewport( 0, 0, w, h );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		double ratio =(double) w /(double) h;
		gluPerspective( 60.0,ratio, 0.0, 1024.0 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

    }

    return MO_DIRECTOR_STATUS_OK;
}

moDirectorStatus moDirectorConsole::ConsoleLoop() {

    if (Initialized()) {
        Interaction();
        Update();
        Draw();
        //Errores::: basado en moDebug, o moTextHeap
        moText plog;
        for(int i =0; i < MODebug2->Count(); i++ ) {
            plog = MODebug2->Pop();
            if (plog.Trim()!=moText("")) Log( plog );
        }
    } else {

        moConfig dummy;
        if (m_pResourceManager) {
            if (!m_pResourceManager->Initialized())
                m_pResourceManager->Init( "../../data/test" , dummy, 0, 800, 600, 800, 600 );

            if (m_pResourceManager->GetModelMan()) {
                m_pResourceManager->GetModelMan()->MoldeoLogo(m_timerticks);
                GLSwapBuffers();
            }


        }

    }
    return MO_DIRECTOR_STATUS_OK;
}
