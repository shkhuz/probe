class Texture {
public:
    int width;
    int height;
    SDL_Texture* texture = NULL;
    SDL_Renderer* renderer;

    bool load_from_file(SDL_Renderer* renderer, std::string path);
    bool load_from_text(
        SDL_Renderer* renderer,
        TTF_Font* font,
        std::string text,
        SDL_Color text_color
    );
    void free();
    void set_color(u8 red, u8 green, u8 blue);
    void set_blend_mode(SDL_BlendMode blend_mode);
    void set_alpha(u8 alpha);
    void render(int x, int y, SDL_Rect* clip);
};

bool Texture::load_from_file(SDL_Renderer* renderer, std::string path) {
    this->renderer = renderer;
    this->free();

    SDL_Texture* new_texture = NULL;
    SDL_Surface* img = IMG_Load(path.c_str());
    if (img == NULL) {
        fprintf(stderr, "Unable to load img '%s': %s\n", path.c_str(), SDL_GetError());
        return false;
    }

    SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 0xff, 0xff));
    new_texture = SDL_CreateTextureFromSurface(renderer, img);
    if (new_texture == NULL) {
        fprintf(
            stderr,
            "Unable to create texture from '%s': %s\n",
            path.c_str(),
            SDL_GetError()
        );
        SDL_FreeSurface(img);
        return false;
    }

    this->width = img->w;
    this->height = img->h;
    SDL_FreeSurface(img);
    this->texture = new_texture;
    return true;
}

bool Texture::load_from_text(
    SDL_Renderer* renderer,
    TTF_Font* font,
    std::string text,
    SDL_Color text_color
) {
    this->renderer = renderer;
    this->free();

    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), text_color);
    if (text_surface == NULL) {
        fprintf(stderr, "Unable to render text surface! %s\n", SDL_GetError());
        return false;
    }

    this->texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (this->texture == NULL) {
        fprintf(stderr, "Unable to create texture from text! %s\n", SDL_GetError());
        return false;
    }

    this->width = text_surface->w;
    this->height = text_surface->h;

    SDL_FreeSurface(text_surface);
    return true;
}

void Texture::free() {
    if (this->texture != NULL) {
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;
    }
}

void Texture::set_color(u8 red, u8 green, u8 blue) {
    SDL_SetTextureColorMod(this->texture, red, green, blue);
}

void Texture::set_blend_mode(SDL_BlendMode blend_mode) {
    SDL_SetTextureBlendMode(this->texture, blend_mode);
}

void Texture::set_alpha(u8 alpha) {
    SDL_SetTextureAlphaMod(this->texture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip) {
    SDL_Rect render_rect = { x, y, this->width, this->height };
    if (clip) {
        render_rect.w = clip->w;
        render_rect.h = clip->h;
    }

    SDL_RenderCopy(this->renderer, this->texture, clip, &render_rect);
}
