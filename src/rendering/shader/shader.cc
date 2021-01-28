#include "shader.hh"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <set>

static GLuint compile_shader(const std::string& shader_str, const GLenum shader_type)
{
    const GLuint shader = glCreateShader(shader_type); TEST_OPENGL_ERROR();
    char const* const shader_cstr = shader_str.c_str();
    glShaderSource(shader, 1, &shader_cstr, nullptr);

    // Compile Shader
    glCompileShader(shader);

    // Check Compilation
    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

    GLint max_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
    std::vector<GLchar> info_log((size_t)(max_length+1));
    glGetShaderInfoLog(shader, max_length, &max_length, info_log.data());

    if (max_length >= 1)
    {
        std::cerr << "[SHADER:" << shader << "][COMPILE INFO]" << (char*)info_log.data() << std::endl; 
    }

    if (is_compiled == GL_FALSE)
    {
        std::cerr << "[SHADER:" << shader << "][COMPILE ERROR] Failed to compile shader." << std::endl;
        glDeleteShader(shader);
        exit(1);
    }

    return shader;
}

static std::string read_file(const std::string& filepath)
{
    std::ifstream f{filepath};
    std::string content;

    if (f.is_open())
    {
        std::basic_ostringstream<char> ss;
        ss << f.rdbuf();

        content = ss.str();
    } else { std::cerr << __FILE__ << ":" << __LINE__ << " attempted reading file: '" << filepath << "' but was not found." << std::endl; }

    return content;
}

static void check_link(GLuint vertex_shader, GLuint fragment_shader, GLuint program_id)
{
    GLint is_linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);

    GLint max_length = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);
    
    std::vector<GLchar> info_log((size_t)(max_length)+1);
    glGetProgramInfoLog(program_id, max_length, &max_length, info_log.data());
    if (max_length >= 1)
    {
        std::cerr << "[SHADER PROGRAM:" << program_id << "][LINK INFO]" << (char*)info_log.data() << std::endl; 
    }

    if (is_linked == GL_FALSE)
    {
        std::cerr << "[SHADER PROGRAM:" << program_id << "][LINK ERROR] Failed to link shader program." << std::endl;
        
        glDeleteProgram(program_id);
        if (vertex_shader != fragment_shader)
        {
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
        } else
        {
            glDeleteShader(vertex_shader);
        }
        
        exit(1);   
    }
}

GLuint Shader::get_current_shader()
{
    GLint current_shader = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader); TEST_OPENGL_ERROR();

    return current_shader;
} 

Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    const std::string vertex_shader_source   = read_file(vertex_shader_path);
    const std::string fragment_shader_source = read_file(fragment_shader_path);

    const auto vertex_shader   = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    const auto fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    bool are_valid_shaders = glIsShader(vertex_shader) && glIsShader(fragment_shader);
    if (!are_valid_shaders) { std::cerr << "[SHADER] Invalid shader ids while building new shader." << std::endl; exit(1); }

    shader_id = glCreateProgram(); TEST_OPENGL_ERROR();
    
    // Attach shaders
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, fragment_shader);

    // Link program
    glLinkProgram(shader_id);

    // Check Link
    check_link(vertex_shader, fragment_shader, shader_id);

    // Shader can be detached
    glDetachShader(shader_id, vertex_shader);
    glDetachShader(shader_id, fragment_shader);
}


Shader::Shader(const std::string& vertex_shader_path, const std::string& geometry_shader_path, const std::string& fragment_shader_path)
{
    const std::string vertex_shader_source   = read_file(vertex_shader_path);
    const std::string geometry_shader_source = read_file(geometry_shader_path);
    const std::string fragment_shader_source = read_file(fragment_shader_path);

    const auto vertex_shader   = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    const auto geometry_shader = compile_shader(geometry_shader_source, GL_GEOMETRY_SHADER);
    const auto fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    bool are_valid_shaders = glIsShader(vertex_shader) && glIsShader(fragment_shader) && glIsShader(geometry_shader);
    if (!are_valid_shaders) { std::cerr << "[SHADER] Invalid shader ids while building new shader." << std::endl; exit(1); }

    shader_id = glCreateProgram(); TEST_OPENGL_ERROR();
    
    // Attach shaders
    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, geometry_shader);
    glAttachShader(shader_id, fragment_shader);

    // Link program
    glLinkProgram(shader_id);

    // Check Link
    check_link(vertex_shader, fragment_shader, shader_id);

    // Shader can be detached
    glDetachShader(shader_id, vertex_shader);
    glDetachShader(shader_id, geometry_shader);
    glDetachShader(shader_id, fragment_shader);
}

void Shader::set_uniform(const std::string& uniform_name, const int value)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniform1i(location, value);
}

void Shader::set_uniform(const std::string& uniform_name, const float value)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());    
    glUniform1f(location, value);
}

void Shader::set_uniform(const std::string& uniform_name, const glm::vec3& value)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::set_uniform(const std::string& uniform_name, const glm::vec4& value)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const std::string& uniform_name, const glm::mat3& value)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniformMatrix3fv(location, 1, GL_TRUE, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& uniform_name, const glm::mat4& value, const unsigned& invert)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniformMatrix4fv(location, 1, invert, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& uniform_name, const std::vector<glm::vec4>& values)
{
    const GLint location = glGetUniformLocation(shader_id, uniform_name.c_str());
    glUniform4fv(location, GLsizei(values.size()), &(values[0].x));
}




void Shader::set_buffer(const std::string& layout_name, const SSBO& buffer)
{
    if (buffer.ssbo_id != SSBO::get_current_ssbo())
        buffer.bind();

    const GLint index = glGetProgramResourceIndex(shader_id, GL_SHADER_STORAGE_BLOCK, layout_name.c_str());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer.ssbo_id); TEST_OPENGL_ERROR();
}

static std::string handle_shader_include(const std::string& filepath, std::set<std::string>& include_files, const int level=0)
{
    auto ret = include_files.insert(filepath);
    if (ret.second == false)
    {
        std::cerr << "[SHADER INCLUDE][WARNING] Max revisiting file: " << filepath << ". Potential infinite loop" << std::endl;
        return std::string{};
    }
    

    std::istringstream f(read_file(filepath));
    std::string source;
    std::string line;

    while (std::getline(f, line))
    {
        auto position = line.find("#include");
        if (position != std::string::npos)
        {
            std::stringstream tokens{line};
            std::string inc, include_filepath;
            tokens >> inc;
            if (inc.substr(0, 2) == "//" || inc.substr(0, 2) == "/*")
                continue;
            tokens >> include_filepath;

            std::string directory;

            auto last_backslash = filepath.rfind("/");
            if (last_backslash == std::string::npos)
                directory = "./";
            else
                directory = filepath.substr(0, last_backslash+1);

            source += handle_shader_include(directory + include_filepath.substr(1, include_filepath.size() - 2), include_files, level + 1);
        } else {
            source += line + "\n";
        }
    }

    // Remove #version
    if (level != 0)
        return source;

    std::string version_string = "#version";
    size_t pos = 0;
    while ((pos = source.find(version_string, pos)) != std::string::npos)
    {
        source.insert(pos, "//");
        pos += version_string.size();
    }

    source.insert(0, "#version 450 core\n");
    return source;
}

ComputeShader::ComputeShader(const std::string& filepath, bool handle_include)
{
    std::string compute_shader_source;
    auto include_set = std::set<std::string>{};
    
    if (!handle_include)
        compute_shader_source = read_file(filepath);
    else
        compute_shader_source = handle_shader_include(filepath, include_set);

    const auto cp_shader = compile_shader(compute_shader_source, GL_COMPUTE_SHADER);

    bool are_valid_shaders = glIsShader(cp_shader);
    if (!are_valid_shaders) { std::cerr << "[COMPUTE SHADER] Invalid shader ids while building new shader." << std::endl; exit(1); }

    shader_id = glCreateProgram(); TEST_OPENGL_ERROR();
    
    // Attach shaders
    glAttachShader(shader_id, cp_shader);
    
    // Link program
    glLinkProgram(shader_id);
    // Check Link
    check_link(cp_shader, cp_shader, shader_id);

    // Shader can be detached
    glDetachShader(shader_id, cp_shader);
}

void ComputeShader::print_workgroups_capabilities()
{
    int workgroup_count[3];
    int workgroup_size[3];
    int workgroup_invocations;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

    std::cout << "*** Compute Shader Info ***" << std::endl;
    std::cout << "=======================================================" << std::endl;
    
    std::cout << "\t [MAX WORKGROUP SIZE]   : " 
              << "x=" << workgroup_count[0] << ", y=" << workgroup_count[1] << ", z=" << workgroup_count[2]
              << std::endl; 

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

    std::cout << "\t [MAX LOCAL INVOKE]     : "
              << "x=" << workgroup_size[0] << ", y=" << workgroup_size[1] << ", z=" << workgroup_size[2]
              << std::endl;

    glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
    std::cout << "\t [MAX WORKGROUP INVOKE] : " << workgroup_invocations << std::endl;
    std::cout << "=======================================================" << std::endl;
}