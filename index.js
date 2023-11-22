import init from './dist/webp.js'

const webp = await init()
const img = document.createElement('img');

img.onload = () => URL.revokeObjectURL(img.src)

document.body.appendChild(img);

const $input = document.getElementById('input');

$input.addEventListener('change', async (e) => {
  const file = e.target.files[0];

  if (!file) return;

  let ext = file.name.split('.').pop();

  webp.FS_createDataFile('/', `input.${ext}`, new Uint8Array(await file.arrayBuffer()))
})

let data = webp.generate();

data = new Uint8Array(Array.from({ length: data.size() }, (_, i) => data.get(i)));

img.src = URL.createObjectURL(new Blob([data], { type: 'image/webp' }));