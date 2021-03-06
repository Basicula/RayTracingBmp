#include <Common/ThreadPool.h>

#ifdef ENABLED_CUDA
#include <CUDACore/CUDAUtils.h>
#include <CUDACore/KernelHandler.h>

#include <Memory/device_ptr.h>
#include <Memory/managed_ptr.h>
#endif

#include <Math/Constants.h>
#include <Math/Vector.h>

#include <Fluid/Fluid.h>

#include <Fractal/MandelbrotSet.h>
#include <Fractal/JuliaSet.h>
#include <Fractal/LyapunovFractal.h>
#include <Fractal/MappingFunctions.h>

#include <Geometry/BoundingBox.h>
#include <Geometry/Sphere.h>
#include <Geometry/Plane.h>
#include <Geometry/Cylinder.h>
#include <Geometry/Torus.h>

#include <Image/Image.h>

#include <Main/SceneExamples.h>
#include <Main/ConsoleLogEventListner.h>
#include <Main/SimpleCameraController.h>

#include <Memory/custom_vector.h>

#include <Visual/SpotLight.h>
#include <Visual/PhongMaterial.h>

#include <Rendering/RenderableObject.h>
#include <Rendering/Scene.h>
#include <Rendering/CPURayTracer.h>
#include <Rendering/SimpleCamera.h>

#include <Window/GLUTWindow.h>
#include <Window/GLFWWindow.h>

#include <BMPWriter/BMPWriter.h>

#ifdef ENABLED_OPENCL
#include <OpenCLCore/OpenCLEnvironment.h>
#include <OpenCLKernels/MandelbrotSetKernel.h>
#endif

#include <Main/CudaTest.h>

#include <iostream>
#include <chrono>

void test_fluid()
  {
  const std::size_t width = 640;
  const std::size_t height = 480;

  Scene scene("Fluid test");
  Image image(width, height, 0xffaaaaaa);

  scene.AddCamera(
    new SimpleCamera(
      Vector3d(0, 5, 5),
      Vector3d(0, -1, -1),
      Vector3d(0, sqrt(2) / 2, -sqrt(2) / 2),
      75,
      width * 1.0 / height), true);
  scene.AddLight(new SpotLight(Vector3d(0, 10, 0)));
  auto fluid = new Fluid(48);
  scene.AddObject(fluid);

  CPURayTracer renderer;
  renderer.SetOutputImage(&image);
  renderer.SetScene(&scene);
  auto update_func = [&]()
    {
    renderer.Render();
    fluid->Update();
    };
#ifdef _DEBUG
  for (int i = 0; i < 5; ++i)
    {
    std::cout << i << std::endl;
    auto start = std::chrono::system_clock::now();
    update_func();
    auto end = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>
      (end - start).count() << std::endl;
    }
#else
  GLUTWindow window(width, height, "Fluid demo");
  window.SetImageSource(&image);
  window.SetUpdateFunction(update_func);
  window.Open();
#endif
  }

void test_scene()
  {
  const std::size_t width = 800;
  const std::size_t height = 600;

  Scene scene = ExampleScene::RandomSpheres(20);

  Image image(width, height);
  CPURayTracer renderer;
  renderer.SetOutputImage(&image);
  renderer.SetScene(&scene);

  auto update_func = [&]()
    {
    renderer.Render();
    };
  //GLUTWindow window(width, height, scene.GetName());
  GLFWWindow window(width, height, scene.GetName());
  window.SetImageSource(&image);
  window.SetUpdateFunction(update_func);
  window.SetEventListner(new SimpleCameraController(scene.GetActiveCamera()));
  window.Open();
  }

#ifdef ENABLED_OPENCL
void test_opencl()
  {
  const std::size_t width = 1024;
  const std::size_t height = 768;
  Image image(width, height);
  OpenCLEnvironment env;
  env.Init();
  env.PrintInfo();
  std::size_t iterations = 1000;
  MandelbrotSetKernel kernel(width, height, iterations);
  kernel.SetMaxIterations(iterations);
  kernel.SetOutput(image);
  FPSCounter fps_counter(std::cout);
  env.Build(kernel);
  const auto update_func = [&]()
    {
    env.Execute(kernel);
    fps_counter.Update();
    };
#if false
  for (auto i = 0; i < 30; ++i)
    update_func();
#else
  GLUTWindow window(width, height, "OpenCLTest");
  window.SetImageSource(&image);
  window.SetUpdateFunction(update_func);
  window.Open();
#endif
  }
#endif

void test_fractals()
  {
  const std::size_t width = 1024;
  const std::size_t height = 768;
  Image image(width, height);
  std::size_t max_iterations = 100;
  MandelbrotSet mandelbrot_set(width, height, max_iterations);
  //JuliaSet julia_set(width, height, max_iterations);
  custom_vector<Color> color_map
    {
    Color(0, 0, 0),
    Color(66, 45, 15),
    Color(25, 7, 25),
    Color(10, 0, 45),
    Color(5, 5, 73),
    Color(0, 7, 99),
    Color(12, 43, 137),
    Color(22, 81, 175),
    Color(56, 124, 209),
    Color(132, 181, 229),
    Color(209, 234, 247),
    Color(239, 232, 191),
    Color(247, 201, 94),
    Color(255, 170, 0),
    Color(204, 127, 0),
    Color(153, 86, 0),
    Color(104, 51, 2),
    };
  //std::vector<Color> color_map
  //  {
  //  Color(0, 0, 0),
  //  Color(0, 0, 255),
  //  Color(0, 255, 0),
  //  Color(255, 0, 0),
  //  };
  //julia_set.SetColorMap(std::make_unique<SmoothColorMap>(color_map));
  //julia_set.SetType(JuliaSet::JuliaSetType::WhiskeryDragon);
  class FractalChangeEventListner : public EventListner {
  public:
    FractalChangeEventListner(Fractal* ip_fractal) : mp_fractal(ip_fractal) {}

    virtual void PollEvents() override {};

  protected:
    virtual void _ProcessEvent(const Event& i_event) override {
      if (i_event.Type() != Event::EventType::KEY_PRESSED_EVENT)
        return;
      const auto& key_pressed_event = static_cast<const KeyPressedEvent&>(i_event);
      switch (key_pressed_event.Key()) {
        case KeyboardButton::KEY_W:
          origin_y += delta / scale;
          break;
        case KeyboardButton::KEY_S:
          origin_y -= delta / scale;
          break;
        case KeyboardButton::KEY_D:
          origin_x += delta / scale;
          break;
        case KeyboardButton::KEY_A:
          origin_x -= delta / scale;
          break;
        case KeyboardButton::KEY_Q:
          scale *= 1.5;
          break;
        case KeyboardButton::KEY_E:
          scale /= 1.5;
          break;
        default:
          break;
      }
      mp_fractal->SetOrigin(origin_x, origin_y);
      mp_fractal->SetScale(scale);
    }
  private:
    Fractal* mp_fractal;
    const float delta = 0.1f;
    float origin_x = 0.0f, origin_y = 0.0f;
    float scale = 1.0f;
  } event_listner(&mandelbrot_set);
  auto update_func = [&]()
    {
    ThreadPool::GetInstance()->ParallelFor(
      static_cast<std::size_t>(0),
      width * height,
      [&](std::size_t i)
      {
      int x = static_cast<int>(i % width);
      int y = static_cast<int>(i / width);
      image.SetPixel(x, y, FractalMapping::Default(mandelbrot_set.GetValue(x, y), color_map));
      //image.SetPixel(x, y, julia_set.GetColor(x, y));
      });
    };
  GLUTWindow window(width, height, "FracralsTest");
  window.SetImageSource(&image);
  window.SetUpdateFunction(update_func);
  window.SetEventListner(&event_listner);
  window.Open();
  }

void test_event_listner() {
  const std::size_t width = 1024;
  const std::size_t height = 768;
  //GLUTWindow window(width, height, "EventListnerTest");
  GLFWWindow window(width, height, "EventListnerTest");
  window.SetEventListner(new ConsoleLogEventListner);
  window.Open();
}

int main()
  {
  //test_fluid();
  test_scene();
  //test();
  //test_opencl();
#ifdef ENABLED_CUDA
  //test_cuda();
#endif
  //test_fractals();
  //test_event_listner();
  return 0;
  }