use minifb::{Key, Window, WindowOptions};

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("rust_bridge.h");

        type RustBridge;
        // FFI call to create the C++ bridge instance.
        fn CreateRustBridge() -> UniquePtr<RustBridge>;

        unsafe fn Init(self: Pin<&mut RustBridge>, native_window: *mut u8) -> bool;
        fn RenderFrame(self: Pin<&mut RustBridge>);
    }
}

fn main() {
    let width = 1024;
    let height = 1024;

    println!("Starting visible on-screen rendering demo...");

    let mut window = Window::new(
        "Impress Demo - Press ESC to exit",
        width,
        height,
        WindowOptions::default(),
    )
    .unwrap();

    let handle = window.get_window_handle();
    let mut demo = ffi::CreateRustBridge();

    // Pass the raw window handle to C++ for Filament to render onto.
    if !unsafe { demo.as_mut().unwrap().Init(handle as *mut u8) } {
        println!("Failed to initialize sample!");
        return;
    }

    window.limit_update_rate(Some(std::time::Duration::from_micros(16600)));

    while window.is_open() && !window.is_key_down(Key::Escape) {
        demo.as_mut().unwrap().RenderFrame();
        window.update();
    }
}
