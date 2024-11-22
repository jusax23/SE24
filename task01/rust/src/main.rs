use png::{ColorType, Encoder};
use std::{fs, str::Split};

#[derive(Debug, Clone, Default)]
struct Color {
    r: u8,
    g: u8,
    b: u8,
}

impl Color {
    fn new(r: u8, g: u8, b: u8) -> Self {
        Self { r, g, b }
    }
}

fn load_file(path: String) -> Option<String> {
    fs::read_to_string(path).ok()
}

fn check_file(iter: &mut Split<'_, &str>) -> bool {
    iter.next().map_or(false, |line| line == "indeximage")
}

fn get_size(iter: &mut Split<'_, &str>) -> Option<(usize, usize)> {
    let width = iter.next().and_then(|line| line.parse().ok())?;
    let height = iter.next().and_then(|line| line.parse().ok())?;
    Some((width, height))
}

fn load_colors(iter: &mut Split<'_, &str>) -> Option<Vec<Color>> {
    let count = iter.next().and_then(|line| line.parse().ok())?;
    let mut palette = Vec::new();
    for _ in 0..count {
        let triplet = iter.next()?;
        let mut line = triplet.split(" ");
        let r = line.next().and_then(|line| line.parse().ok())?;
        let g = line.next().and_then(|line| line.parse().ok())?;
        let b = line.next().and_then(|line| line.parse().ok())?;

        palette.push(Color::new(r, g, b));
    }
    Some(palette)
}

fn create_image(
    iter: &mut Split<'_, &str>,
    palette: &[Color],
    size: (usize, usize),
) -> Option<Vec<u8>> {
    let mut data = Vec::with_capacity(size.0 * size.1);
    for _ in 0..size.1 {
        let line = iter.next()?;
        let mut count = 0;
        for color_name in line.split(" ") {
            count += 1;
            let color_id: usize = color_name.parse().ok()?;
            let Color { r, g, b } = palette.get(color_id)?;
            data.push(*r);
            data.push(*g);
            data.push(*b);
        }
        if count != size.0 {
            return None;
        }
    }
    Some(data)
}

fn write_buffer(
    file: &str,
    data: &[u8],
    size: (usize, usize),
) -> Result<(), Box<dyn std::error::Error>> {
    let file = fs::File::create(file)?;
    let writer = std::io::BufWriter::new(file);

    let mut encoder = Encoder::new(writer, size.0 as u32, size.1 as u32);
    encoder.set_color(ColorType::Rgb);
    encoder.set_depth(png::BitDepth::Eight);
    let mut png_writer = encoder.write_header()?;
    png_writer.write_image_data(data)?;
    Ok(())
}

fn main() {
    let file = load_file("data.dat".to_owned());
    let Some(file) = file else {
        println!("Error while opening file.");
        return;
    };
    let mut iter = file.split("\n");
    if !check_file(&mut iter) {
        println!("File has wrong type.");
        return;
    }
    let Some(size) = get_size(&mut iter) else {
        println!("Could not get size.");
        return;
    };
    let Some(palette) = load_colors(&mut iter) else {
        println!("Could not load color pallet.");
        return;
    };
    let Some(data) = create_image(&mut iter, &palette, size) else {
        println!("Error while creating image.");
        return;
    };
    if write_buffer("image.png", &data, size).is_err() {
        println!("Error while saving file.");
        return;
    }
    println!("Success");
}
