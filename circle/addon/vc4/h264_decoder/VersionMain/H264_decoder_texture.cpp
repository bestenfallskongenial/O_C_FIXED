inline bool CH264Decoder::CheckGLError()
{
                GLenum error = glGetError();
                if (error != GL_NO_ERROR)
                    {
                    const char* error_str;
                    
                    switch(error) 
                        {
                        case        GL_INVALID_ENUM:                    error_str = "GL_INVALID_ENUM"; break;
                        case        GL_INVALID_VALUE:                   error_str = "GL_INVALID_VALUE"; break;
                        case        GL_INVALID_OPERATION:               error_str = "GL_INVALID_OPERATION"; break;
                        case        GL_OUT_OF_MEMORY:                   error_str = "GL_OUT_OF_MEMORY"; break;
                        case        GL_INVALID_FRAMEBUFFER_OPERATION:   error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                        default:                                        error_str = "UNKNOWN_ERROR"; break;
                        }
                    MMALstoreLog (error_str);     
                    return false;
                    }
                return true;    
}

bool            CH264Decoder::MMALcreateTextures       (   )
{
                int count = 0;

                glGenTextures(1, &m_TextureA);
                if(!CheckGLError()) count++;
                glBindTexture(GL_TEXTURE_2D, m_TextureA);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!CheckGLError()) count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!CheckGLError()) count++;

                if( count != 0)
                    { 
                    MMALstoreLog("\nTexture A Creation FAILED");
                    return false;
                    }
                count = 0;
                 
                glGenTextures(1, &m_TextureB);
                if(!CheckGLError()) count++;
                glBindTexture(GL_TEXTURE_2D, m_TextureB);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                if(!CheckGLError()) count++;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                if(!CheckGLError()) count++;
                glBindTexture(GL_TEXTURE_2D, 0);
                if(!CheckGLError()) count++;

                if( count != 0)
                    { 
                    MMALstoreLog("\nTexture B Creation FAILED");
                    return false;
                    }
                MMALstoreLog("\nTexture Creation SUCCESS");
                return true;
}